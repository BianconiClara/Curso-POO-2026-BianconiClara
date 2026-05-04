#ifndef APPCONTROLLER_H
#define APPCONTROLLER_H

#include "editorprincipal.h"
#include "loginscreen.h"
#include "modobloqueado.h"

#include <QObject>
#include <memory>

class AppController : public QObject
{
    Q_OBJECT

public:
    explicit AppController(QObject *parent = nullptr);
    void start();

private slots:
    void onLoginSuccess(const QString &userName);
    void onLockRequested(int seconds);
    void onLockFinished();

private:
    void mostrarPantalla(Pantalla *pantalla, bool fullScreen = false);

    AppConfig m_config;
    std::unique_ptr<LoginScreen> m_loginScreen;
    std::unique_ptr<ModoBloqueado> m_blockedScreen;
    std::unique_ptr<EditorPrincipal> m_editorScreen;
    Pantalla *m_currentScreen = nullptr;
};

#endif // APPCONTROLLER_H
