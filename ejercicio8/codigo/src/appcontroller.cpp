#include "appcontroller.h"

#include "appconfig.h"
#include "logger.h"

AppController::AppController(QObject *parent)
    : QObject(parent)
    , m_config(AppConfigLoader::load())
    , m_loginScreen(std::make_unique<LoginScreen>(m_config))
    , m_blockedScreen(std::make_unique<ModoBloqueado>(m_config))
    , m_editorScreen(std::make_unique<EditorPrincipal>(m_config))
{
    Logger::instance().initialize(m_config.logFilePath);
    Logger::instance().log(QString("Aplicacion iniciada. Configuracion cargada desde %1")
                           .arg(m_config.configPath));

    connect(m_loginScreen.get(), &LoginScreen::loginExitoso,
            this, &AppController::onLoginSuccess);
    connect(m_loginScreen.get(), &LoginScreen::bloqueoSolicitado,
            this, &AppController::onLockRequested);
    connect(m_blockedScreen.get(), &ModoBloqueado::bloqueoFinalizado,
            this, &AppController::onLockFinished);
}

void AppController::start()
{
    mostrarPantalla(m_loginScreen.get());
}

void AppController::onLoginSuccess(const QString &userName)
{
    m_editorScreen->setUsuarioActivo(userName);
    Logger::instance().log(QString("Login valido para el usuario %1. Se abre el editor en pantalla completa.")
                           .arg(userName));
    mostrarPantalla(m_editorScreen.get(), true);
}

void AppController::onLockRequested(int seconds)
{
    m_blockedScreen->iniciarBloqueo(seconds);
    Logger::instance().log(QString("Se activa ModoBloqueado por %1 segundos.").arg(seconds));
    mostrarPantalla(m_blockedScreen.get());
}

void AppController::onLockFinished()
{
    m_loginScreen->restablecerFormulario();
    Logger::instance().log("Finalizo el bloqueo temporal. Vuelve la pantalla de login.");
    mostrarPantalla(m_loginScreen.get());
}

void AppController::mostrarPantalla(Pantalla *pantalla, bool fullScreen)
{
    if (pantalla == nullptr) {
        return;
    }

    pantalla->cargarDatos();
    pantalla->validarEstado();
    pantalla->registrarEvento("Pantalla activada por el controlador.");

    if (fullScreen) {
        pantalla->showFullScreen();
    } else {
        pantalla->showNormal();
        pantalla->show();
    }

    pantalla->raise();
    pantalla->activateWindow();

    if (m_currentScreen != nullptr && m_currentScreen != pantalla) {
        m_currentScreen->hide();
    }

    m_currentScreen = pantalla;
}
