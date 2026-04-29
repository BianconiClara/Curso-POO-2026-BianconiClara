#include "appconfig.h"

#include <QCoreApplication>
#include <QDir>
#include <QFileInfo>
#include <QSettings>

namespace
{
QString ensureDirectory(const QString &path)
{
    QDir().mkpath(path);
    return QFileInfo(path).absoluteFilePath();
}

QString readTextSetting(QSettings &settings, const QString &key, const QString &fallback)
{
    const QString value = settings.value(key, fallback).toString().trimmed();
    return value.isEmpty() ? fallback : value;
}

QString findConfigUpwards(const QString &basePath)
{
    if (basePath.trimmed().isEmpty()) {
        return QString();
    }

    QDir directory(basePath);
    while (true) {
        const QString candidate = directory.filePath("config/app.ini");
        if (QFileInfo::exists(candidate)) {
            return QFileInfo(candidate).absoluteFilePath();
        }

        if (!directory.cdUp()) {
            break;
        }
    }

    return QString();
}

QString resolveDirectoryFromConfig(const QString &configFilePath,
                                   const QString &configuredPath,
                                   const QString &fallbackRelativePath)
{
    QFileInfo configInfo(configFilePath);
    const QDir configDir = configInfo.dir();

    const QString relativeOrAbsolute = configuredPath.trimmed().isEmpty()
        ? fallbackRelativePath
        : configuredPath.trimmed();

    const QString resolvedPath = QDir::isAbsolutePath(relativeOrAbsolute)
        ? relativeOrAbsolute
        : configDir.filePath(relativeOrAbsolute);

    return ensureDirectory(QFileInfo(resolvedPath).absoluteFilePath());
}
}

QString AppConfigLoader::configFilePath()
{
    const QString fromAppDir = findConfigUpwards(QCoreApplication::applicationDirPath());
    if (!fromAppDir.isEmpty()) {
        return fromAppDir;
    }

    const QString fromCurrentDir = findConfigUpwards(QDir::currentPath());
    if (!fromCurrentDir.isEmpty()) {
        return fromCurrentDir;
    }

    return QDir(QCoreApplication::applicationDirPath()).filePath("config/app.ini");
}

QString AppConfigLoader::projectRootPath()
{
    const QFileInfo configInfo(configFilePath());
    QDir directory = configInfo.dir();
    if (directory.dirName().compare("config", Qt::CaseInsensitive) == 0) {
        directory.cdUp();
    }
    return directory.absolutePath();
}

QString AppConfigLoader::logsDirectoryPath()
{
    return ensureDirectory(QDir(projectRootPath()).filePath("logs"));
}

QString AppConfigLoader::defaultExportDirectoryPath()
{
    return ensureDirectory(QDir(projectRootPath()).filePath("exports"));
}

AppConfig AppConfigLoader::load()
{
    AppConfig config;
    config.configPath = configFilePath();
    config.projectRootPath = projectRootPath();

    QSettings settings(config.configPath, QSettings::IniFormat);
    config.initialUser = readTextSetting(settings, "login/usuario_inicial", config.initialUser);
    config.initialPassword = readTextSetting(settings, "login/clave_inicial", config.initialPassword);
    config.defaultLanguage = readTextSetting(settings, "editor/lenguaje_por_defecto", config.defaultLanguage);

    config.lockSeconds = settings.value("login/tiempo_bloqueo_segundos", config.lockSeconds).toInt();
    if (config.lockSeconds <= 0) {
        config.lockSeconds = 15;
    }

    const QString configuredExportPath = settings.value("editor/ruta_exportacion").toString();
    config.exportDirectory = resolveDirectoryFromConfig(config.configPath,
                                                        configuredExportPath,
                                                        "../exports");

    if (config.defaultLanguage != "C++"
        && config.defaultLanguage != "Python"
        && config.defaultLanguage != "Java") {
        config.defaultLanguage = "C++";
    }

    config.logFilePath = QDir(logsDirectoryPath()).filePath("editor_multilenguaje.log");
    return config;
}
