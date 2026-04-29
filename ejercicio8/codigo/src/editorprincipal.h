#ifndef EDITORPRINCIPAL_H
#define EDITORPRINCIPAL_H

#include "pantalla.h"
#include "validadorsintaxis.h"

#include <QHash>
#include <QLabel>
#include <QSet>
#include <memory>

class QComboBox;
class QPlainTextEdit;
class QPushButton;
class QSplitter;

class EditorPrincipal : public Pantalla
{
    Q_OBJECT

public:
    explicit EditorPrincipal(const AppConfig &config, QWidget *parent = nullptr);

    QString nombrePantalla() const override;
    void inicializarUI() override;
    void conectarEventos() override;
    void cargarDatos() override;
    bool validarEstado() override;
    void registrarEvento(const QString &descripcion) override;
    void setUsuarioActivo(const QString &userName);

protected:
    bool eventFilter(QObject *watched, QEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;
    void closeEvent(QCloseEvent *event) override;
    void focusInEvent(QFocusEvent *event) override;
    void focusOutEvent(QFocusEvent *event) override;

private slots:
    void cambiarLenguaje(const QString &language);
    void exportarJpg();
    void cerrarVentana();
    void cargarPlantillaActual();
    void actualizarContextoCursor();
    void marcarLineaActualComoEditada();
    void validarLineaActual();

private:
    void actualizarValidadorActual();
    bool manejarAtajo(QKeyEvent *event);
    QString plantillaParaLenguaje(const QString &language) const;
    void establecerContenidoPlantilla(const QString &language);
    void validarLinea(int blockNumber);
    void aplicarResaltadoLineas();
    void actualizarPanelEstado(const QString &message, bool error = false);
    void actualizarResumenPosicion();
    void limpiarErrores();
    QWidget *crearTarjetaInfo(const QString &title, const QString &body);
    void actualizarVistaLateral();
    bool exportarCodigoComoImagen(QString *savedPath);

    std::unique_ptr<ValidadorCpp> m_validadorCpp;
    std::unique_ptr<ValidadorPython> m_validadorPython;
    std::unique_ptr<ValidadorJava> m_validadorJava;
    ValidadorSintaxis *m_validadorActual = nullptr;

    QLabel *m_badgeLabel = nullptr;
    QLabel *m_titleLabel = nullptr;
    QLabel *m_statusLabel = nullptr;
    QLabel *m_diagnosticLabel = nullptr;
    QLabel *m_userLabel = nullptr;
    QLabel *m_cursorLabel = nullptr;
    QComboBox *m_languageCombo = nullptr;
    QPushButton *m_exportButton = nullptr;
    QPushButton *m_closeButton = nullptr;
    QPushButton *m_validateButton = nullptr;
    QPushButton *m_templateButton = nullptr;
    QPlainTextEdit *m_editor = nullptr;
    QSplitter *m_splitter = nullptr;
    QWidget *m_sidebarCard = nullptr;
    QLabel *m_photoLabel = nullptr;

    QHash<int, QString> m_erroresPorLinea;
    QSet<int> m_lineasEditadas;
    QString m_usuarioActivo;
    QString m_lenguajeActual;
    int m_lineaActual = -1;
    bool m_documentoModificado = false;
    bool m_inicializado = false;
    bool m_suprimirPromptCambioLenguaje = false;
};

#endif // EDITORPRINCIPAL_H
