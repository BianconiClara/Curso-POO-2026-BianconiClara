#include "validadorsintaxis.h"

#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QProcess>
#include <QRegularExpression>
#include <QSet>
#include <QStandardPaths>
#include <QTemporaryDir>
#include <QVector>

namespace
{
ValidationResult ok(const QString &message = "Linea valida.")
{
    return {true, message};
}

ValidationResult error(const QString &message)
{
    return {false, message};
}

bool tieneOperadorAsignacion(const QString &texto)
{
    return QRegularExpression("(^|[^=!<>])=([^=]|$)").match(texto).hasMatch();
}

bool delimitadoresBalanceados(const QString &linea)
{
    QVector<QChar> pila;
    bool enComillaSimple = false;
    bool enComillaDoble = false;
    bool escapando = false;

    for (const QChar caracter : linea) {
        if (escapando) {
            escapando = false;
            continue;
        }

        if (caracter == '\\') {
            escapando = true;
            continue;
        }

        if (!enComillaDoble && caracter == '\'') {
            enComillaSimple = !enComillaSimple;
            continue;
        }

        if (!enComillaSimple && caracter == '"') {
            enComillaDoble = !enComillaDoble;
            continue;
        }

        if (enComillaSimple || enComillaDoble) {
            continue;
        }

        if (caracter == '(' || caracter == '[' || caracter == '{') {
            pila.append(caracter);
            continue;
        }

        if (caracter == ')' || caracter == ']' || caracter == '}') {
            if (pila.isEmpty()) {
                return false;
            }

            const QChar apertura = pila.takeLast();
            const bool coincide =
                (apertura == '(' && caracter == ')') ||
                (apertura == '[' && caracter == ']') ||
                (apertura == '{' && caracter == '}');

            if (!coincide) {
                return false;
            }
        }
    }

    return !enComillaSimple && !enComillaDoble && pila.isEmpty();
}

DocumentValidationResult validarDocumentoPorLineas(const ValidadorSintaxis &validador,
                                                   const QString &contenido,
                                                   const QString &summaryMessage = {})
{
    DocumentValidationResult result;
    const QStringList lineas = contenido.split('\n');

    for (int i = 0; i < lineas.size(); ++i) {
        const ValidationResult lineResult = validador.validarLinea(lineas.at(i));
        if (!lineResult.valid) {
            result.errorsByLine.insert(i, lineResult.message);
        }
    }

    result.summaryMessage = summaryMessage.isEmpty()
        ? QString("Validacion completada para %1.").arg(validador.lenguaje())
        : summaryMessage;
    return result;
}

QString encontrarCompiladorCpp()
{
    QStringList candidatos;

    const QString compiladorConfigurado = QString::fromLocal8Bit(qgetenv("CXX")).trimmed();
    if (!compiladorConfigurado.isEmpty()) {
        candidatos << compiladorConfigurado;
    }

    candidatos << QStandardPaths::findExecutable("g++")
               << QStandardPaths::findExecutable("clang++")
               << QStandardPaths::findExecutable("c++");

#ifdef Q_OS_WIN
    candidatos << "C:/Qt/Tools/mingw1310_64/bin/g++.exe"
               << "C:/msys64/ucrt64/bin/g++.exe"
               << "C:/msys64/mingw64/bin/g++.exe"
               << "C:/MinGW/bin/g++.exe";

    const QDir qtToolsDir("C:/Qt/Tools");
    const QFileInfoList toolchains = qtToolsDir.entryInfoList(
        QStringList() << "mingw*" << "llvm*",
        QDir::Dirs | QDir::NoDotAndDotDot,
        QDir::Name);

    for (const QFileInfo &toolchain : toolchains) {
        const QDir toolchainDir(toolchain.absoluteFilePath());
        candidatos << toolchainDir.filePath("bin/g++.exe")
                   << toolchainDir.filePath("bin/clang++.exe");
    }
#endif

    QSet<QString> revisados;
    for (const QString &candidato : candidatos) {
        if (candidato.isEmpty()) {
            continue;
        }

        const QString rutaNormalizada = QDir::cleanPath(candidato);
        if (revisados.contains(rutaNormalizada)) {
            continue;
        }

        revisados.insert(rutaNormalizada);

        const QFileInfo info(rutaNormalizada);
        if (info.exists() && info.isFile() && info.isExecutable()) {
            return info.absoluteFilePath();
        }
    }

    return {};
}

void agregarErrorDeCompilador(QHash<int, QString> &errorsByLine,
                              int lineNumber,
                              const QString &message)
{
    if (lineNumber < 0 || message.isEmpty() || errorsByLine.contains(lineNumber)) {
        return;
    }

    errorsByLine.insert(lineNumber, message);
}

ValidationResult validarLineaCppHeuristica(const QString &linea)
{
    const QString texto = linea.trimmed();

    if (texto.isEmpty() || texto.startsWith("//")) {
        return ok("Linea valida para C++.");
    }

    if (!delimitadoresBalanceados(texto)) {
        return error("Revisa parentesis, llaves o corchetes en esta linea de C++.");
    }

    if (QRegularExpression("^include(\\b|\\s|<|\")").match(texto).hasMatch()) {
        return error("En C++ debes escribir la directiva como '#include ...'. Falta el '#'.");
    }

    if (texto == "include" || texto == "#include") {
        return error("La directiva #include esta incompleta. Usa #include <archivo> o #include \"archivo\".");
    }

    if (texto.startsWith("#include")) {
        if (QRegularExpression("^#include\\s+(<[^<>\\s]+>|\"[^\"]+\")$").match(texto).hasMatch()) {
            return ok("Directiva #include valida.");
        }

        return error("Formato invalido de #include. Usa #include <archivo> o #include \"archivo\".");
    }

    if (QRegularExpression("^#(define|ifdef|ifndef|if|elif|else|endif|pragma|undef|error|line)\\b.*$")
            .match(texto).hasMatch()) {
        return ok("Directiva de preprocesador valida.");
    }

    if (texto.startsWith('#')) {
        return error("Directiva de preprocesador no reconocida. Revisa la instruccion despues de '#'.");
    }

    if (texto == "{" || texto == "}" || texto == "};") {
        return ok("Bloque correctamente delimitado.");
    }

    if (QRegularExpression("^(public|private|protected)\\s*:$").match(texto).hasMatch()) {
        return ok("Especificador de acceso valido.");
    }

    if (QRegularExpression("^(case\\s+.+|default)\\s*:$").match(texto).hasMatch()) {
        return ok("Etiqueta de switch valida.");
    }

    if (QRegularExpression("^(if|for|while|switch|catch)\\s*\\(.*\\)\\s*\\{?$").match(texto).hasMatch()
        || QRegularExpression("^else(\\s+if\\s*\\(.*\\))?\\s*\\{?$").match(texto).hasMatch()) {
        return ok("Control de flujo valido para C++.");
    }

    if (QRegularExpression("^(class|struct|enum|namespace)\\b.*(\\{|;)?$").match(texto).hasMatch()) {
        if (texto.endsWith('{') || texto.endsWith(';')) {
            return ok("Declaracion de tipo o namespace valida.");
        }
        return error("Una declaracion de C++ normalmente termina con ';' o abre bloque con '{'.");
    }

    if (QRegularExpression("^using\\s+namespace\\s+[A-Za-z_][\\w:]*;$").match(texto).hasMatch()) {
        return ok("Sentencia using namespace valida.");
    }

    if (QRegularExpression("^using\\s+namespace\\s+[A-Za-z_][\\w:]*$").match(texto).hasMatch()) {
        return error("La sentencia 'using namespace ...' en C++ debe terminar con ';'.");
    }

    if (QRegularExpression("^[A-Za-z_~][\\w:<>*&\\s]*\\(.*\\)\\s*(const)?\\s*\\{?$").match(texto).hasMatch()) {
        return ok("Firma de funcion valida.");
    }

    if (QRegularExpression("^return\\b.*$").match(texto).hasMatch()) {
        return texto.endsWith(';')
            ? ok("Sentencia return valida.")
            : error("La sentencia return en C++ debe terminar con ';'.");
    }

    if (QRegularExpression("^((std::)?(cout|cerr|clog))\\s*<<.*$").match(texto).hasMatch()) {
        return texto.endsWith(';')
            ? ok("Salida por consola valida.")
            : error("Una linea con cout/cerr/clog debe terminar con ';'.");
    }

    if (tieneOperadorAsignacion(texto) && !texto.endsWith(';') && !texto.endsWith('{')) {
        return error("Parece una declaracion o asignacion de C++ y deberia terminar con ';'.");
    }

    if (texto.endsWith(';') || texto.endsWith('{')) {
        return ok("Sentencia valida para C++.");
    }

    return error("En C++ esta linea suele terminar en ';' o abrir un bloque con '{'.");
}

DocumentValidationResult validarDocumentoCppConHeuristica(const QString &contenido,
                                                          const QString &summaryMessage)
{
    DocumentValidationResult result;
    const QStringList lineas = contenido.split('\n');

    for (int i = 0; i < lineas.size(); ++i) {
        const ValidationResult lineResult = validarLineaCppHeuristica(lineas.at(i));
        if (!lineResult.valid) {
            result.errorsByLine.insert(i, lineResult.message);
        }
    }

    result.summaryMessage = summaryMessage;
    return result;
}
}

DocumentValidationResult ValidadorSintaxis::validarDocumento(const QString &contenido) const
{
    return validarDocumentoPorLineas(*this, contenido);
}

QString ValidadorCpp::lenguaje() const
{
    return "C++";
}

ValidationResult ValidadorCpp::validarLinea(const QString &linea) const
{
    return validarLineaCppHeuristica(linea);
}

DocumentValidationResult ValidadorCpp::validarDocumento(const QString &contenido) const
{
    const QString compilador = encontrarCompiladorCpp();
    if (compilador.isEmpty()) {
        return validarDocumentoCppConHeuristica(
            contenido,
            "No se encontro un compilador C++; se uso la validacion basica por linea.");
    }

    const QString nombreCompilador = QFileInfo(compilador).fileName();
    QTemporaryDir tempDir;
    if (!tempDir.isValid()) {
        return validarDocumentoCppConHeuristica(
            contenido,
            QString("No se pudo crear un archivo temporal para %1; se uso la validacion basica.")
                .arg(nombreCompilador));
    }

    QFile sourceFile(tempDir.filePath("validacion.cpp"));
    if (!sourceFile.open(QIODevice::WriteOnly | QIODevice::Text)) {
        return validarDocumentoCppConHeuristica(
            contenido,
            "No se pudo escribir el archivo temporal de C++; se uso la validacion basica.");
    }

    sourceFile.write(contenido.toUtf8());
    sourceFile.close();

    QProcess process;
    process.setProgram(compilador);
    process.setArguments({
        "-std=c++17",
        "-fsyntax-only",
        "-fdiagnostics-color=never",
        "-finput-charset=UTF-8",
        QDir::toNativeSeparators(sourceFile.fileName())
    });
    process.start();

    if (!process.waitForStarted(3000)) {
        return validarDocumentoCppConHeuristica(
            contenido,
            QString("No se pudo iniciar %1; se uso la validacion basica por linea.")
                .arg(nombreCompilador));
    }

    if (!process.waitForFinished(8000)) {
        process.kill();
        process.waitForFinished();
        return validarDocumentoCppConHeuristica(
            contenido,
            QString("La validacion con %1 supero el tiempo esperado; se uso la validacion basica.")
                .arg(nombreCompilador));
    }

    const QString diagnostico = QString::fromLocal8Bit(process.readAllStandardError())
        + QString::fromLocal8Bit(process.readAllStandardOutput());

    DocumentValidationResult result;
    const QRegularExpression compilerErrorPattern(
        "^(.+?):(\\d+):(?:(\\d+):)?\\s*(fatal error|error):\\s*(.+)$");

    const QStringList lineasDiagnostico = diagnostico.split('\n', Qt::SkipEmptyParts);
    for (const QString &lineaDiagnostico : lineasDiagnostico) {
        const QRegularExpressionMatch match = compilerErrorPattern.match(lineaDiagnostico.trimmed());
        if (!match.hasMatch()) {
            continue;
        }

        const int lineNumber = match.captured(2).toInt() - 1;
        const QString message = match.captured(5).trimmed();
        agregarErrorDeCompilador(result.errorsByLine, lineNumber, message);
    }

    if (process.exitStatus() != QProcess::NormalExit) {
        return validarDocumentoCppConHeuristica(
            contenido,
            QString("%1 finalizo de forma inesperada; se uso la validacion basica por linea.")
                .arg(nombreCompilador));
    }

    if (process.exitCode() == 0) {
        result.summaryMessage = QString("Compilacion de sintaxis OK con %1.")
            .arg(nombreCompilador);
        return result;
    }

    if (result.errorsByLine.isEmpty()) {
        return validarDocumentoCppConHeuristica(
            contenido,
            QString("%1 devolvio un diagnostico no asociado a una linea; se uso la validacion basica.")
                .arg(nombreCompilador));
    }

    result.summaryMessage = QString("Diagnostico actualizado con %1.")
        .arg(nombreCompilador);
    return result;
}

QString ValidadorPython::lenguaje() const
{
    return "Python";
}

ValidationResult ValidadorPython::validarLinea(const QString &linea) const
{
    const QString texto = linea.trimmed();

    if (texto.isEmpty() || texto.startsWith('#')) {
        return ok("Linea valida para Python.");
    }

    if (!delimitadoresBalanceados(texto)) {
        return error("Revisa parentesis, corchetes o llaves en esta linea de Python.");
    }

    if (texto.startsWith("#include") || QRegularExpression("^include(\\b|\\s|<|\")").match(texto).hasMatch()) {
        return error("Python no usa #include. Para importar modulos usa 'import' o 'from ... import ...'.");
    }

    if (texto.contains(';')) {
        return error("En Python no hace falta cerrar la linea con ';'.");
    }

    if (texto.endsWith('{') || texto == "}" || texto == "{") {
        return error("Python usa indentacion y ':' en lugar de llaves.");
    }

    if (texto == "import") {
        return error("La sentencia import esta incompleta. Indica el modulo a importar.");
    }

    if (texto == "from" || QRegularExpression("^from\\s+\\S+$").match(texto).hasMatch()) {
        return error("Completa la sentencia como 'from modulo import elemento'.");
    }

    if (QRegularExpression("^(if|elif|else|for|while|def|class|try|except|finally|with|match|case)\\b").match(texto).hasMatch()) {
        return texto.endsWith(':')
            ? ok("Cabecera de bloque valida.")
            : error("En Python los bloques se abren con ':'.");
    }

    if (QRegularExpression("^(return|yield|pass|break|continue|raise|assert|del|global|nonlocal)\\b.*$")
            .match(texto).hasMatch()) {
        return ok("Sentencia valida para Python.");
    }

    if (QRegularExpression("^(import\\s+.+|from\\s+.+\\s+import\\s+.+)$").match(texto).hasMatch()) {
        return ok("Import valido.");
    }

    if (QRegularExpression("^print\\s+.+$").match(texto).hasMatch()) {
        return error("En Python 3 conviene escribir print con parentesis: print(...).");
    }

    if (QRegularExpression("^@[A-Za-z_][\\w\\.]*").match(texto).hasMatch()) {
        return ok("Decorador valido.");
    }

    if (QRegularExpression("^[A-Za-z_][\\w\\.\\[\\]]*\\s*([+\\-*/%]?=)\\s*.+$").match(texto).hasMatch()) {
        return ok("Asignacion valida.");
    }

    if (QRegularExpression("^[A-Za-z_][\\w\\.]*\\s*\\(.*\\)\\s*$").match(texto).hasMatch()) {
        return ok("Llamada a funcion valida.");
    }

    if (QRegularExpression("^(\\[.*\\]|\\{.*\\}|\\(.*\\))$").match(texto).hasMatch()) {
        return ok("Literal compuesto valido.");
    }

    return error("La linea no coincide con una estructura Python reconocible. Revisa ':' o la forma de la expresion.");
}

QString ValidadorJava::lenguaje() const
{
    return "Java";
}

ValidationResult ValidadorJava::validarLinea(const QString &linea) const
{
    const QString texto = linea.trimmed();

    if (texto.isEmpty()
        || texto.startsWith("//")
        || texto.startsWith("/*")
        || texto.startsWith('*')
        || texto.startsWith("*/")) {
        return ok("Linea valida para Java.");
    }

    if (!delimitadoresBalanceados(texto)) {
        return error("Revisa parentesis, llaves o corchetes en esta linea de Java.");
    }

    if (texto.startsWith("#include") || QRegularExpression("^include(\\b|\\s|<|\")").match(texto).hasMatch()) {
        return error("Java no usa #include. Para dependencias utiliza 'import paquete.Clase;'.");
    }

    if (texto == "{" || texto == "}" || texto == "};") {
        return ok("Bloque correctamente delimitado.");
    }

    if (texto.startsWith('@')) {
        return ok("Anotacion valida.");
    }

    if (texto == "import") {
        return error("La sentencia import esta incompleta. Indica el paquete o clase.");
    }

    if (QRegularExpression("^import\\s+.+[^;]$").match(texto).hasMatch()) {
        return error("En Java los import deben terminar con ';'.");
    }

    if (texto == "package") {
        return error("La sentencia package esta incompleta. Indica el nombre del paquete.");
    }

    if (QRegularExpression("^package\\s+.+[^;]$").match(texto).hasMatch()) {
        return error("En Java la sentencia package debe terminar con ';'.");
    }

    if (QRegularExpression("^(case\\s+.+|default)\\s*:$").match(texto).hasMatch()) {
        return ok("Etiqueta de switch valida.");
    }

    if (QRegularExpression("^(if|for|while|switch|catch|synchronized)\\s*\\(.*\\)\\s*\\{?$").match(texto).hasMatch()
        || QRegularExpression("^else\\s*\\{?$").match(texto).hasMatch()
        || QRegularExpression("^do\\s*\\{?$").match(texto).hasMatch()) {
        return ok("Control de flujo valido para Java.");
    }

    if (QRegularExpression("^(package|import)\\b.*;$").match(texto).hasMatch()) {
        return ok("Sentencia de paquete o import valida.");
    }

    if (QRegularExpression("^(public\\s+)?(class|interface|enum|record)\\b.*\\{?$").match(texto).hasMatch()) {
        return texto.endsWith('{')
            ? ok("Declaracion de tipo valida.")
            : error("En Java las clases, interfaces o records suelen abrir bloque con '{'.");
    }

    if (QRegularExpression("^[A-Za-z_][\\w<>\\[\\]\\s,]*\\s+[A-Za-z_][\\w]*\\s*\\(.*\\)\\s*(throws\\s+[\\w\\.,\\s]+)?\\s*\\{?$")
            .match(texto).hasMatch()) {
        return ok("Firma de metodo valida.");
    }

    if (QRegularExpression("^return\\b.*$").match(texto).hasMatch()) {
        return texto.endsWith(';')
            ? ok("Sentencia return valida.")
            : error("La sentencia return en Java debe terminar con ';'.");
    }

    if (QRegularExpression("^System\\.out\\.(print|println)\\s*\\(.*\\)$").match(texto).hasMatch()) {
        return error("La llamada a System.out.print/println debe terminar con ';'.");
    }

    if (tieneOperadorAsignacion(texto) && !texto.endsWith(';') && !texto.endsWith('{')) {
        return error("Parece una declaracion o asignacion de Java y deberia terminar con ';'.");
    }

    if (texto.endsWith(';') || texto.endsWith('{')) {
        return ok("Sentencia valida para Java.");
    }

    return error("En Java la linea suele terminar con ';' o abrir/cerrar un bloque con llaves.");
}
