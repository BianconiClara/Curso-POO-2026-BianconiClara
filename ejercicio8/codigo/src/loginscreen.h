#ifndef LOGINSCREEN_H
#define LOGINSCREEN_H

#include "pantalla.h"

#include <QLabel>
#include <QLineEdit>
#include <QPushButton>

class LoginScreen : public Pantalla
{
    Q_OBJECT

public:
    explicit LoginScreen(const AppConfig &config, QWidget *parent = nullptr);

    QString nombrePantalla() const override;
    void inicializarUI() override;
    void conectarEventos() override;
    void cargarDatos() override;
    bool validarEstado() override;
    void registrarEvento(const QString &descripcion) override;

public slots:
    void intentarLogin();
    void restablecerFormulario();

signals:
    void loginExitoso(const QString &userName);
    void bloqueoSolicitado(int seconds);

protected:
    void keyPressEvent(QKeyEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;
    void closeEvent(QCloseEvent *event) override;
    void focusInEvent(QFocusEvent *event) override;
    void focusOutEvent(QFocusEvent *event) override;

private:
    void actualizarMensaje(const QString &mensaje, bool error = false);
    void actualizarTipografia();

    QLabel *m_badgeLabel = nullptr;
    QLabel *m_titleLabel = nullptr;
    QLabel *m_subtitleLabel = nullptr;
    QLabel *m_statusLabel = nullptr;
    QLabel *m_userHintLabel = nullptr;
    QLineEdit *m_userEdit = nullptr;
    QLineEdit *m_passwordEdit = nullptr;
    QPushButton *m_loginButton = nullptr;
    int m_failedAttempts = 0;
};

#endif // LOGINSCREEN_H
