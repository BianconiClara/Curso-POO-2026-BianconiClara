#include "appcontroller.h"
#include "appconfig.h"
#include "logger.h"

#include <QDir>

AppController::AppController(QObject *parent)
    : QObject(parent)
    , m_config(AppConfigLoader::load())
{
    Logger::instance().initialize(QDir(AppConfigLoader::logsDirectoryPath()).filePath("ejercicio7.log"));
    Logger::instance().log(QString("Aplicacion iniciada. Configuracion leida desde %1").arg(m_config.configPath));

    m_networkService = new NetworkService(m_config, this);
    m_climaWidget = new ClimaWidget();
    m_loginWindow = new LoginWindow(m_config.lockSeconds);
    m_loginWindow->setWeatherWidget(m_climaWidget);

    connect(m_loginWindow, SIGNAL(loginValidated()), this, SLOT(onLoginValidated()));
    connect(m_networkService, SIGNAL(weatherUpdated(WeatherInfo)),
            m_climaWidget, SLOT(setWeatherInfo(WeatherInfo)));
    connect(m_networkService, SIGNAL(backgroundReady(QString,bool)),
            this, SLOT(onBackgroundReady(QString,bool)));
    connect(m_networkService, SIGNAL(backgroundError(QString)),
            this, SLOT(onBackgroundError(QString)));
}

void AppController::start()
{
    m_loginWindow->show();
    m_climaWidget->showLoadingMessage("Consultando temperatura y hora local...");
    m_networkService->requestWeather();
    m_networkService->requestBackground();
}

void AppController::onLoginValidated()
{
    m_loginApproved = true;
    Logger::instance().log("Usuario validado. Esperando fondo principal para abrir la ventana.");

    if (m_backgroundPrepared) {
        openMainWindow();
        return;
    }

    m_loginWindow->setBackgroundPending(true);
}

void AppController::onBackgroundReady(QString localPath, bool fromInternet)
{
    m_backgroundPrepared = true;
    m_backgroundPath = localPath;

    Logger::instance().log(QString("Fondo listo en %1. Origen: %2")
                           .arg(localPath)
                           .arg(fromInternet ? "internet" : "cache/local"));

    if (m_loginApproved) {
        openMainWindow();
    }
}

void AppController::onBackgroundError(QString message)
{
    if (m_loginWindow != nullptr) {
        m_loginWindow->showUserMessage(message, false);
    }
}

void AppController::openMainWindow()
{
    if (m_mainWindow != nullptr) {
        return;
    }

    m_loginWindow->setBackgroundPending(false);

    m_mainWindow = new MainWindow(m_backgroundPath);
    m_mainWindow->showFullScreen();
    m_loginWindow->hide();

    Logger::instance().log("Ventana principal abierta en pantalla completa.");
}
