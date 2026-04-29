#include "validadorsintaxis.h"

#include <QRegularExpression>
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
}

QString ValidadorCpp::lenguaje() const
{
    return "C++";
}

ValidationResult ValidadorCpp::validarLinea(const QString &linea) const
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
