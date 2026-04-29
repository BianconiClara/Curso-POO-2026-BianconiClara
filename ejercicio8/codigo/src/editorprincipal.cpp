#include "editorprincipal.h"

#include "logger.h"

#include <QCloseEvent>
#include <QComboBox>
#include <QDateTime>
#include <QDir>
#include <QEvent>
#include <QFileInfo>
#include <QFocusEvent>
#include <QFontDatabase>
#include <QFrame>
#include <QHBoxLayout>
#include <QImage>
#include <QKeyEvent>
#include <QKeySequence>
#include <QMessageBox>
#include <QMouseEvent>
#include <QPainter>
#include <QPixmap>
#include <QPlainTextEdit>
#include <QPushButton>
#include <QResizeEvent>
#include <QSignalBlocker>
#include <QSplitter>
#include <QTextBlock>
#include <QTextCursor>
#include <QTextEdit>
#include <QTimer>
#include <QVBoxLayout>

namespace
{
QString expandirTabs(const QString &line)
{
    QString result = line;
    result.replace('\t', "    ");
    return result;
}

QTextEdit::ExtraSelection crearSeleccionDeLinea(QPlainTextEdit *editor,
                                                int blockNumber,
                                                const QColor &background,
                                                const QColor &foreground = QColor())
{
    QTextEdit::ExtraSelection selection;
    QTextBlock block = editor->document()->findBlockByNumber(blockNumber);
    QTextCursor cursor(block);
    cursor.clearSelection();

    selection.cursor = cursor;
    selection.format.setProperty(QTextFormat::FullWidthSelection, true);
    selection.format.setBackground(background);
    if (foreground.isValid()) {
        selection.format.setForeground(foreground);
    }
    return selection;
}
}

EditorPrincipal::EditorPrincipal(const AppConfig &config, QWidget *parent)
    : Pantalla(config, parent)
    , m_validadorCpp(std::make_unique<ValidadorCpp>())
    , m_validadorPython(std::make_unique<ValidadorPython>())
    , m_validadorJava(std::make_unique<ValidadorJava>())
{
    inicializarUI();
    conectarEventos();
    cargarDatos();
}

QString EditorPrincipal::nombrePantalla() const
{
    return "EditorPrincipal";
}

void EditorPrincipal::inicializarUI()
{
    setObjectName("editorPrincipal");
    setWindowTitle("Ejercicio 08 - Editor multilenguaje");
    setMinimumSize(1200, 760);
    setFocusPolicy(Qt::StrongFocus);
    setAttribute(Qt::WA_StyledBackground, true);

    setStyleSheet(
        "QWidget#editorPrincipal {"
        "  background:qlineargradient(x1:0, y1:0, x2:1, y2:1,"
        "    stop:0 #0d1b24, stop:0.48 #15313a, stop:1 #20555a);"
        "}"
        "QFrame#editorCard, QFrame#sidebarCard {"
        "  background-color: rgba(248, 243, 235, 235);"
        "  border-radius: 26px;"
        "  border: 1px solid rgba(255, 255, 255, 70);"
        "}"
        "QComboBox, QPushButton {"
        "  min-height: 40px;"
        "  border-radius: 14px;"
        "  font-size: 13px;"
        "}"
        "QComboBox {"
        "  background: white;"
        "  border: 1px solid #d0c4b4;"
        "  padding: 0 12px;"
        "}"
        "QPushButton {"
        "  background-color: #be6a45;"
        "  color: white;"
        "  border: none;"
        "  padding: 0 16px;"
        "  font-weight: 700;"
        "}"
        "QPushButton:hover {"
        "  background-color: #cc7c58;"
        "}"
        "QPlainTextEdit {"
        "  background: #fffdf9;"
        "  color: #19333c;"
        "  border: 1px solid #d6c9b8;"
        "  border-radius: 18px;"
        "  padding: 16px;"
        "  selection-background-color: #c7e6eb;"
        "}"
    );

    QVBoxLayout *pageLayout = new QVBoxLayout(this);
    pageLayout->setContentsMargins(26, 22, 26, 22);
    pageLayout->setSpacing(18);

    QHBoxLayout *headerLayout = new QHBoxLayout;
    headerLayout->setSpacing(12);

    m_badgeLabel = new QLabel("Ejercicio 08 | Offline | Qt Widgets", this);
    m_badgeLabel->setStyleSheet(
        "background-color: rgba(255, 255, 255, 30);"
        "color: #d6eef0;"
        "border-radius: 16px;"
        "padding: 10px 14px;"
        "font-size: 12px;"
        "font-weight: 700;"
    );

    m_titleLabel = new QLabel("Editor multilenguaje con validacion por linea", this);
    m_titleLabel->setStyleSheet("color: #f5f0e8; font-weight: 800;");

    m_statusLabel = new QLabel("Preparando editor...", this);
    m_statusLabel->setStyleSheet(
        "background-color: rgba(255, 255, 255, 28);"
        "color: #dceceb;"
        "border-radius: 16px;"
        "padding: 10px 14px;"
        "font-size: 12px;"
    );

    headerLayout->addWidget(m_badgeLabel, 0, Qt::AlignLeft);
    headerLayout->addWidget(m_titleLabel, 1);
    headerLayout->addWidget(m_statusLabel, 0, Qt::AlignRight);

    pageLayout->addLayout(headerLayout);

    m_splitter = new QSplitter(Qt::Horizontal, this);
    m_splitter->setChildrenCollapsible(false);
    pageLayout->addWidget(m_splitter, 1);

    QFrame *editorCard = new QFrame(m_splitter);
    editorCard->setObjectName("editorCard");

    QVBoxLayout *editorLayout = new QVBoxLayout(editorCard);
    editorLayout->setContentsMargins(24, 22, 24, 22);
    editorLayout->setSpacing(16);

    QHBoxLayout *toolbarLayout = new QHBoxLayout;
    toolbarLayout->setSpacing(10);

    m_userLabel = new QLabel("Sesion: sin iniciar", editorCard);
    m_userLabel->setStyleSheet("color: #36545a; font-size: 13px; font-weight: 700;");

    QLabel *languageLabel = new QLabel("Lenguaje:", editorCard);
    languageLabel->setStyleSheet("color: #5d6666; font-size: 13px;");

    m_languageCombo = new QComboBox(editorCard);
    m_languageCombo->addItems({"C++", "Python", "Java"});
    m_languageCombo->setMinimumWidth(150);

    m_validateButton = new QPushButton("Validar linea", editorCard);
    m_templateButton = new QPushButton("Cargar plantilla", editorCard);
    m_exportButton = new QPushButton("Exportar JPG", editorCard);
    m_closeButton = new QPushButton("Cerrar ventana", editorCard);
    m_closeButton->setStyleSheet(
        "QPushButton {"
        "  background-color: #8d3a30;"
        "  color: white;"
        "  border: none;"
        "  padding: 0 16px;"
        "  font-weight: 700;"
        "  border-radius: 14px;"
        "}"
        "QPushButton:hover {"
        "  background-color: #a64a3f;"
        "}"
    );

    toolbarLayout->addWidget(m_userLabel);
    toolbarLayout->addStretch();
    toolbarLayout->addWidget(languageLabel);
    toolbarLayout->addWidget(m_languageCombo);
    toolbarLayout->addWidget(m_validateButton);
    toolbarLayout->addWidget(m_templateButton);
    toolbarLayout->addWidget(m_exportButton);
    toolbarLayout->addWidget(m_closeButton);

    m_editor = new QPlainTextEdit(editorCard);
    QFont codeFont = QFontDatabase::systemFont(QFontDatabase::FixedFont);
    codeFont.setPointSize(11);
    m_editor->setFont(codeFont);
    m_editor->setLineWrapMode(QPlainTextEdit::NoWrap);
    m_editor->setPlaceholderText("Escribe codigo aqui...");
    m_editor->setTabStopDistance(QFontMetricsF(codeFont).horizontalAdvance(' ') * 4);

    QHBoxLayout *footerLayout = new QHBoxLayout;
    footerLayout->setSpacing(12);

    m_diagnosticLabel = new QLabel("La validacion se ejecuta cuando abandonas la linea en edicion.", editorCard);
    m_diagnosticLabel->setWordWrap(true);
    m_diagnosticLabel->setStyleSheet(
        "background-color: #eee4d8;"
        "color: #6b5d52;"
        "border-radius: 16px;"
        "padding: 12px 14px;"
    );

    m_cursorLabel = new QLabel("Linea 1, columna 1 | 0 error(es)", editorCard);
    m_cursorLabel->setStyleSheet("color: #587075; font-size: 12px; font-weight: 700;");

    footerLayout->addWidget(m_diagnosticLabel, 1);
    footerLayout->addWidget(m_cursorLabel, 0, Qt::AlignRight | Qt::AlignVCenter);

    editorLayout->addLayout(toolbarLayout);
    editorLayout->addWidget(m_editor, 1);
    editorLayout->addLayout(footerLayout);

    QFrame *sidebarCard = new QFrame(m_splitter);
    sidebarCard->setObjectName("sidebarCard");
    m_sidebarCard = sidebarCard;

    QVBoxLayout *sidebarLayout = new QVBoxLayout(sidebarCard);
    sidebarLayout->setContentsMargins(22, 22, 22, 22);
    sidebarLayout->setSpacing(14);

    m_photoLabel = new QLabel(sidebarCard);
    m_photoLabel->setFixedSize(142, 142);
    m_photoLabel->setAlignment(Qt::AlignCenter);
    m_photoLabel->setStyleSheet(
        "background-color: rgba(32, 85, 90, 35);"
        "border-radius: 71px;"
        "border: 3px solid rgba(32, 85, 90, 80);"
    );

    const QPixmap profilePixmap(":/profile/professional-profile.jpeg");
    if (!profilePixmap.isNull()) {
        m_photoLabel->setPixmap(profilePixmap.scaled(m_photoLabel->size(),
                                                     Qt::KeepAspectRatioByExpanding,
                                                     Qt::SmoothTransformation));
    } else {
        m_photoLabel->setText("CB");
    }

    QLabel *profileBadge = new QLabel("Panel lateral estilo LinkedIn", sidebarCard);
    profileBadge->setStyleSheet("color: #2f7d74; font-size: 12px; font-weight: 700;");

    QLabel *profileName = new QLabel("Clara Bianconi", sidebarCard);
    profileName->setStyleSheet("color: #16343d; font-size: 24px; font-weight: 800;");

    QLabel *profileHeadline = new QLabel(
        "Estudiante de Ingenieria en Computacion - Python, C++, HTML, CSS,Java",
        sidebarCard);
    profileHeadline->setWordWrap(true);
    profileHeadline->setStyleSheet("color: #55686d; font-size: 14px; font-weight: 700;");

    QLabel *profileSummary = new QLabel(
        "Perfil orientado al desarrollo de software, con interes en machine learning, "
        "My strongest interests are in software engineering and automation and"
       " I can adapt quickly to new technologies and programming languages. "
        ,
        sidebarCard);
    profileSummary->setWordWrap(true);
    profileSummary->setStyleSheet("color: #5c6667; font-size: 13px;");

    sidebarLayout->addWidget(m_photoLabel, 0, Qt::AlignHCenter);
    sidebarLayout->addWidget(profileBadge);
    sidebarLayout->addWidget(profileName);
    sidebarLayout->addWidget(profileHeadline);
    sidebarLayout->addWidget(profileSummary);
    sidebarLayout->addWidget(crearTarjetaInfo(
        "About Me",
        "I’m a Computer Engineering student with knowledge of Python, data structures and fundamentals of Machine Learning."));
      sidebarLayout->addWidget(crearTarjetaInfo(
        "Contacto",
        "clarabianconi02@gmail.com\n351 506 9143\nCordoba, Argentina"));
    sidebarLayout->addStretch();

    actualizarVistaLateral();
}

void EditorPrincipal::conectarEventos()
{
    connect(m_languageCombo, &QComboBox::currentTextChanged, this, &EditorPrincipal::cambiarLenguaje);
    connect(m_exportButton, &QPushButton::clicked, this, &EditorPrincipal::exportarJpg);
    connect(m_closeButton, &QPushButton::clicked, this, &EditorPrincipal::cerrarVentana);
    connect(m_validateButton, &QPushButton::clicked, this, &EditorPrincipal::validarLineaActual);
    connect(m_templateButton, &QPushButton::clicked, this, &EditorPrincipal::cargarPlantillaActual);
    connect(m_editor, &QPlainTextEdit::cursorPositionChanged, this, &EditorPrincipal::actualizarContextoCursor);
    connect(m_editor, &QPlainTextEdit::textChanged, this, &EditorPrincipal::marcarLineaActualComoEditada);

    m_editor->installEventFilter(this);
    m_editor->viewport()->installEventFilter(this);
}

void EditorPrincipal::cargarDatos()
{
    m_userLabel->setText(QString("Sesion: %1")
                         .arg(m_usuarioActivo.isEmpty() ? m_config.initialUser : m_usuarioActivo));

    if (!m_inicializado) {
        m_suprimirPromptCambioLenguaje = true;
        const int index = qMax(0, m_languageCombo->findText(m_config.defaultLanguage));
        m_languageCombo->setCurrentIndex(index);
        m_suprimirPromptCambioLenguaje = false;

        actualizarValidadorActual();
        establecerContenidoPlantilla(m_languageCombo->currentText());
        actualizarPanelEstado("Plantilla inicial cargada. Cambia de linea para disparar la validacion.");
        m_inicializado = true;
    } else {
        actualizarValidadorActual();
        actualizarResumenPosicion();
    }
}

bool EditorPrincipal::validarEstado()
{
    const bool ready = m_editor != nullptr
        && m_languageCombo != nullptr
        && m_validadorActual != nullptr;

    if (m_exportButton != nullptr) {
        m_exportButton->setEnabled(ready && !m_editor->toPlainText().trimmed().isEmpty());
    }

    return ready;
}

void EditorPrincipal::registrarEvento(const QString &descripcion)
{
    Logger::instance().log(QString("[%1] %2").arg(nombrePantalla(), descripcion));
}

void EditorPrincipal::setUsuarioActivo(const QString &userName)
{
    m_usuarioActivo = userName;
    if (m_userLabel != nullptr) {
        m_userLabel->setText(QString("Sesion: %1").arg(userName));
    }
}

bool EditorPrincipal::eventFilter(QObject *watched, QEvent *event)
{
    if (watched == m_editor) {
        if (event->type() == QEvent::FocusIn) {
            actualizarPanelEstado("Editor activo. Usa Ctrl+S para exportar o Ctrl+Enter para validar la linea.");
        } else if (event->type() == QEvent::FocusOut) {
            validarLineaActual();
            actualizarPanelEstado("Se valido la linea actual al salir del editor.");
        } else if (event->type() == QEvent::KeyPress) {
            if (manejarAtajo(static_cast<QKeyEvent *>(event))) {
                return true;
            }
        }
    }

    if (watched == m_editor->viewport() && event->type() == QEvent::MouseButtonPress) {
        QTimer::singleShot(0, this, &EditorPrincipal::actualizarContextoCursor);
        actualizarPanelEstado("Click detectado: puedes mover el cursor o seleccionar texto.");
    }

    return QWidget::eventFilter(watched, event);
}

void EditorPrincipal::keyPressEvent(QKeyEvent *event)
{
    if (manejarAtajo(event)) {
        return;
    }

    QWidget::keyPressEvent(event);
}

void EditorPrincipal::mousePressEvent(QMouseEvent *event)
{
    const QRect editorRect = m_editor == nullptr
        ? QRect()
        : QRect(m_editor->mapTo(this, QPoint(0, 0)), m_editor->size());

    if (m_editor != nullptr && !editorRect.contains(event->position().toPoint())) {
        actualizarPanelEstado("Panel lateral activo. La ultima linea editada se validara cuando cambies de foco.");
    }

    QWidget::mousePressEvent(event);
}

void EditorPrincipal::resizeEvent(QResizeEvent *event)
{
    QFont titleFont("Bahnschrift SemiBold", width() > 1400 ? 24 : 20);
    titleFont.setBold(true);
    m_titleLabel->setFont(titleFont);

    actualizarVistaLateral();
    QWidget::resizeEvent(event);
}

void EditorPrincipal::closeEvent(QCloseEvent *event)
{
    QMessageBox::StandardButton answer = QMessageBox::No;

    if (m_documentoModificado) {
        answer = QMessageBox::question(
            this,
            "Salir del editor",
            "Hay cambios en el editor. Deseas exportar el codigo a JPG antes de salir?",
            QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel,
            QMessageBox::Yes);

        if (answer == QMessageBox::Cancel) {
            registrarEvento("Cierre cancelado desde el editor.");
            event->ignore();
            return;
        }

        if (answer == QMessageBox::Yes) {
            QString path;
            if (!exportarCodigoComoImagen(&path)) {
                event->ignore();
                return;
            }
        }
    } else {
        answer = QMessageBox::question(
            this,
            "Salir del editor",
            "Deseas cerrar la aplicacion?",
            QMessageBox::Yes | QMessageBox::No,
            QMessageBox::No);

        if (answer != QMessageBox::Yes) {
            registrarEvento("Cierre cancelado desde el editor.");
            event->ignore();
            return;
        }
    }

    registrarEvento("Cierre confirmado desde el editor.");
    event->accept();
}

void EditorPrincipal::focusInEvent(QFocusEvent *event)
{
    actualizarPanelEstado("Ventana principal activa. La app sigue funcionando totalmente offline.");
    registrarEvento("La ventana principal recibio el foco.");
    QWidget::focusInEvent(event);
}

void EditorPrincipal::focusOutEvent(QFocusEvent *event)
{
    validarLineaActual();
    registrarEvento("La ventana principal perdio el foco.");
    QWidget::focusOutEvent(event);
}

void EditorPrincipal::cambiarLenguaje(const QString &language)
{
    m_lenguajeActual = language;
    actualizarValidadorActual();
    limpiarErrores();
    actualizarResumenPosicion();

    if (m_suprimirPromptCambioLenguaje) {
        return;
    }

    registrarEvento(QString("Lenguaje activo cambiado a %1.").arg(language));

    if (!m_editor->toPlainText().trimmed().isEmpty()) {
        const auto answer = QMessageBox::question(
            this,
            "Cambiar lenguaje",
            QString("Deseas cargar una plantilla base para %1? El contenido actual se reemplazara.")
                .arg(language),
            QMessageBox::Yes | QMessageBox::No,
            QMessageBox::No);

        if (answer == QMessageBox::Yes) {
            establecerContenidoPlantilla(language);
            actualizarPanelEstado(QString("Plantilla base de %1 cargada.").arg(language));
            return;
        }
    }

    actualizarPanelEstado(QString("Validador cambiado a %1. El codigo actual se mantiene.").arg(language));
}

void EditorPrincipal::exportarJpg()
{
    QString savedPath;
    if (!exportarCodigoComoImagen(&savedPath)) {
        return;
    }

    actualizarPanelEstado(QString("Codigo exportado correctamente a %1.").arg(savedPath));
    QMessageBox::information(this, "Exportacion completada",
                             QString("El archivo JPG se guardo en:\n%1").arg(savedPath));
}

void EditorPrincipal::cerrarVentana()
{
    registrarEvento("Se presiono el boton de cierre del editor.");
    close();
}

void EditorPrincipal::cargarPlantillaActual()
{
    establecerContenidoPlantilla(m_languageCombo->currentText());
    actualizarPanelEstado(QString("Se restauro la plantilla base de %1.").arg(m_languageCombo->currentText()));
    registrarEvento(QString("Plantilla base recargada para %1.").arg(m_languageCombo->currentText()));
}

void EditorPrincipal::actualizarContextoCursor()
{
    const int nuevaLinea = m_editor->textCursor().blockNumber();

    if (m_lineaActual != -1 && m_lineaActual != nuevaLinea) {
        validarLinea(m_lineaActual);
    }

    m_lineaActual = nuevaLinea;
    actualizarResumenPosicion();
    aplicarResaltadoLineas();
}

void EditorPrincipal::marcarLineaActualComoEditada()
{
    const int linea = m_editor->textCursor().blockNumber();
    m_lineasEditadas.insert(linea);
    m_documentoModificado = true;
    validarEstado();
    actualizarResumenPosicion();
}

void EditorPrincipal::validarLineaActual()
{
    const int linea = m_editor->textCursor().blockNumber();
    validarLinea(linea);
}

void EditorPrincipal::actualizarValidadorActual()
{
    const QString language = m_languageCombo->currentText();

    if (language == "Python") {
        m_validadorActual = m_validadorPython.get();
    } else if (language == "Java") {
        m_validadorActual = m_validadorJava.get();
    } else {
        m_validadorActual = m_validadorCpp.get();
    }

    m_lenguajeActual = language;
}

bool EditorPrincipal::manejarAtajo(QKeyEvent *event)
{
    if (event->matches(QKeySequence::Save)) {
        exportarJpg();
        event->accept();
        return true;
    }

    if (event->modifiers().testFlag(Qt::ControlModifier)
        && (event->key() == Qt::Key_Return || event->key() == Qt::Key_Enter)) {
        validarLineaActual();
        actualizarPanelEstado("Validacion manual ejecutada sobre la linea actual.");
        event->accept();
        return true;
    }

    if (event->modifiers().testFlag(Qt::ControlModifier) && event->key() == Qt::Key_L) {
        m_languageCombo->setFocus();
        actualizarPanelEstado("Selector de lenguaje enfocado.");
        event->accept();
        return true;
    }

    if (event->modifiers().testFlag(Qt::ControlModifier) && event->key() == Qt::Key_R) {
        cargarPlantillaActual();
        event->accept();
        return true;
    }

    return false;
}

QString EditorPrincipal::plantillaParaLenguaje(const QString &language) const
{
    if (language == "Python") {
        return
            "def saludar(nombre):\n"
            "    if nombre:\n"
            "        print(f\"Hola, {nombre}\")\n"
            "    else:\n"
            "        print(\"Hola, mundo\")\n"
            "\n"
            "saludar(\"Qt\")\n";
    }

    if (language == "Java") {
        return
            "public class HolaMundo {\n"
            "    public static void main(String[] args) {\n"
            "        if (args.length > 0) {\n"
            "            System.out.println(\"Hola, \" + args[0]);\n"
            "        } else {\n"
            "            System.out.println(\"Hola, mundo\");\n"
            "        }\n"
            "    }\n"
            "}\n";
    }

    return
        "#include <iostream>\n"
        "\n"
        "int main() {\n"
        "    const std::string nombre = \"Qt\";\n"
        "    if (!nombre.empty()) {\n"
        "        std::cout << \"Hola, \" << nombre << std::endl;\n"
        "    } else {\n"
        "        std::cout << \"Hola, mundo\" << std::endl;\n"
        "    }\n"
        "    return 0;\n"
        "}\n";
}

void EditorPrincipal::establecerContenidoPlantilla(const QString &language)
{
    const QSignalBlocker blocker(m_editor);
    m_editor->setPlainText(plantillaParaLenguaje(language));
    m_editor->moveCursor(QTextCursor::Start);

    m_lineaActual = m_editor->textCursor().blockNumber();
    m_documentoModificado = false;
    limpiarErrores();
    validarEstado();
    actualizarResumenPosicion();
}

void EditorPrincipal::validarLinea(int blockNumber)
{
    if (m_validadorActual == nullptr) {
        return;
    }

    const QTextBlock block = m_editor->document()->findBlockByNumber(blockNumber);
    if (!block.isValid()) {
        return;
    }

    const ValidationResult result = m_validadorActual->validarLinea(block.text());

    if (result.valid) {
        if (m_erroresPorLinea.remove(blockNumber) > 0) {
            registrarEvento(QString("La linea %1 volvio a ser valida para %2.")
                            .arg(blockNumber + 1)
                            .arg(m_validadorActual->lenguaje()));
        }

        if (blockNumber == m_editor->textCursor().blockNumber()) {
            actualizarPanelEstado(QString("Linea %1 valida para %2.")
                                  .arg(blockNumber + 1)
                                  .arg(m_validadorActual->lenguaje()));
        }
    } else {
        const QString previousMessage = m_erroresPorLinea.value(blockNumber);
        m_erroresPorLinea.insert(blockNumber, result.message);

        if (previousMessage != result.message) {
            registrarEvento(QString("Error de sintaxis en linea %1: %2")
                            .arg(blockNumber + 1)
                            .arg(result.message));
        }

        if (blockNumber == m_editor->textCursor().blockNumber()) {
            actualizarPanelEstado(QString("Linea %1: %2").arg(blockNumber + 1).arg(result.message), true);
        }
    }

    m_lineasEditadas.remove(blockNumber);
    aplicarResaltadoLineas();
    actualizarResumenPosicion();
}

void EditorPrincipal::aplicarResaltadoLineas()
{
    QList<QTextEdit::ExtraSelection> selections;

    for (auto it = m_erroresPorLinea.cbegin(); it != m_erroresPorLinea.cend(); ++it) {
        selections.append(crearSeleccionDeLinea(m_editor, it.key(), QColor("#ffd7d2"), QColor("#6e2218")));
    }

    const int currentLine = m_editor->textCursor().blockNumber();
    if (!m_erroresPorLinea.contains(currentLine)) {
        selections.append(crearSeleccionDeLinea(m_editor, currentLine, QColor("#dff1f4")));
    }

    m_editor->setExtraSelections(selections);
}

void EditorPrincipal::actualizarPanelEstado(const QString &message, bool error)
{
    m_diagnosticLabel->setText(message);
    m_diagnosticLabel->setStyleSheet(QString(
        "background-color:%1;"
        "color:%2;"
        "border-radius:16px;"
        "padding:12px 14px;")
        .arg(error ? "#f9d9d5" : "#eee4d8",
             error ? "#8c3124" : "#6b5d52"));
}

void EditorPrincipal::actualizarResumenPosicion()
{
    const QTextCursor cursor = m_editor->textCursor();
    m_cursorLabel->setText(QString("Linea %1, columna %2 | %3 error(es)")
                           .arg(cursor.blockNumber() + 1)
                           .arg(cursor.positionInBlock() + 1)
                           .arg(m_erroresPorLinea.size()));

    const QString usuario = m_usuarioActivo.isEmpty() ? m_config.initialUser : m_usuarioActivo;
    m_statusLabel->setText(QString("%1 | Usuario: %2 | Exporta en %3")
                           .arg(m_lenguajeActual,
                                usuario,
                                QFileInfo(m_config.exportDirectory).fileName()));
}

void EditorPrincipal::limpiarErrores()
{
    m_erroresPorLinea.clear();
    m_lineasEditadas.clear();
    aplicarResaltadoLineas();
}

QWidget *EditorPrincipal::crearTarjetaInfo(const QString &title, const QString &body)
{
    QFrame *card = new QFrame(m_sidebarCard);
    card->setStyleSheet(
        "QFrame {"
        "  background-color: rgba(17, 49, 57, 18);"
        "  border: 1px solid rgba(23, 49, 57, 35);"
        "  border-radius: 18px;"
        "}"
    );

    QVBoxLayout *layout = new QVBoxLayout(card);
    layout->setContentsMargins(16, 14, 16, 14);
    layout->setSpacing(8);

    QLabel *titleLabel = new QLabel(title, card);
    titleLabel->setStyleSheet("color: #173842; font-size: 13px; font-weight: 800;");

    QLabel *bodyLabel = new QLabel(body, card);
    bodyLabel->setWordWrap(true);
    bodyLabel->setStyleSheet("color: #5a6768; font-size: 13px;");

    layout->addWidget(titleLabel);
    layout->addWidget(bodyLabel);
    return card;
}

void EditorPrincipal::actualizarVistaLateral()
{
    if (m_sidebarCard == nullptr) {
        return;
    }

    m_sidebarCard->setMaximumWidth(qBound(320, width() / 3, 430));
}

bool EditorPrincipal::exportarCodigoComoImagen(QString *savedPath)
{
    const QString code = m_editor->toPlainText();
    if (code.trimmed().isEmpty()) {
        actualizarPanelEstado("No hay codigo para exportar.", true);
        QMessageBox::warning(this, "Exportacion no disponible", "Escribe o deja una plantilla cargada antes de exportar.");
        return false;
    }

    QDir().mkpath(m_config.exportDirectory);

    const QString fileName = QString("codigo_%1_%2.jpg")
        .arg(m_lenguajeActual.toLower().replace('+', "p"))
        .arg(QDateTime::currentDateTime().toString("yyyyMMdd_hhmmss"));
    const QString fullPath = QDir(m_config.exportDirectory).filePath(fileName);

    const QStringList lines = code.split('\n');

    QFont titleFont("Bahnschrift SemiBold", 18);
    titleFont.setBold(true);
    QFont metaFont("Aptos", 10);
    QFont codeFont = QFontDatabase::systemFont(QFontDatabase::FixedFont);
    codeFont.setPointSize(11);

    QFontMetrics titleMetrics(titleFont);
    QFontMetrics metaMetrics(metaFont);
    QFontMetrics codeMetrics(codeFont);

    int maxWidth = 0;
    for (const QString &line : lines) {
        maxWidth = qMax(maxWidth, codeMetrics.horizontalAdvance(expandirTabs(line)));
    }

    const int digits = QString::number(qMax(1, lines.size())).size();
    const int gutterWidth = codeMetrics.horizontalAdvance(QString(digits, '9')) + 26;
    const int lineHeight = codeMetrics.height() + 8;
    const int headerHeight = 110;
    const int footerHeight = 28;
    const int padding = 34;
    const int codeAreaWidth = gutterWidth + maxWidth + 40;
    const int imageWidth = qMax(1100, codeAreaWidth + (padding * 2));
    const int imageHeight = headerHeight + footerHeight + (lines.size() * lineHeight) + (padding * 2);

    QImage image(imageWidth, imageHeight, QImage::Format_RGB32);
    image.fill(QColor("#f6f1e8"));

    QPainter painter(&image);
    painter.setRenderHint(QPainter::Antialiasing, true);
    painter.setRenderHint(QPainter::TextAntialiasing, true);

    painter.fillRect(image.rect(), QColor("#f7f2ea"));
    painter.fillRect(QRect(0, 0, imageWidth, 90), QColor("#183840"));

    painter.setPen(QColor("#eef7f6"));
    painter.setFont(titleFont);
    painter.drawText(QRect(padding, 26, imageWidth - (padding * 2), titleMetrics.height() + 8),
                     Qt::AlignLeft | Qt::AlignVCenter,
                     QString("Codigo exportado | %1").arg(m_lenguajeActual));

    painter.setPen(QColor("#c9ddda"));
    painter.setFont(metaFont);
    painter.drawText(QRect(padding, 58, imageWidth - (padding * 2), metaMetrics.height() + 4),
                     Qt::AlignLeft | Qt::AlignVCenter,
                     QString("Usuario: %1 | Fecha: %2 | Archivo JPG unico generado por la app")
                         .arg(m_usuarioActivo.isEmpty() ? m_config.initialUser : m_usuarioActivo,
                              QDateTime::currentDateTime().toString("dd/MM/yyyy hh:mm:ss")));

    const QRect codeRect(padding, headerHeight, imageWidth - (padding * 2), imageHeight - headerHeight - padding);
    painter.setPen(Qt::NoPen);
    painter.setBrush(QColor("#fffdf9"));
    painter.drawRoundedRect(codeRect, 18, 18);

    painter.setFont(codeFont);

    for (int i = 0; i < lines.size(); ++i) {
        const int y = headerHeight + 24 + (i * lineHeight);
        const QRect lineRect(padding + 10, y - codeMetrics.ascent(), codeRect.width() - 20, lineHeight);

        if (m_erroresPorLinea.contains(i)) {
            painter.fillRect(lineRect.adjusted(0, -3, 0, 3), QColor("#fde0dd"));
        }

        painter.setPen(QColor("#9e8879"));
        painter.drawText(padding + 18,
                         y,
                         QString::number(i + 1).rightJustified(digits, ' '));

        painter.setPen(QColor("#14313a"));
        painter.drawText(padding + 18 + gutterWidth,
                         y,
                         expandirTabs(lines.at(i)));
    }

    if (!image.save(fullPath, "JPG", 95)) {
        actualizarPanelEstado("No se pudo guardar el JPG exportado.", true);
        QMessageBox::warning(this, "Error al exportar", "Qt no pudo escribir el archivo JPG en la ruta configurada.");
        return false;
    }

    if (savedPath != nullptr) {
        *savedPath = fullPath;
    }

    m_documentoModificado = false;
    registrarEvento(QString("Codigo exportado a %1.").arg(fullPath));
    validarEstado();
    return true;
}
