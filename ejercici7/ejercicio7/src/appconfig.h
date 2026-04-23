#ifndef APPCONFIG_H
#define APPCONFIG_H

#include "models.h"

class AppConfigLoader
{
public:
    static AppConfig load();
    static QString configFilePath();
    static QString dataRootPath();
    static QString cacheDirectoryPath();
    static QString logsDirectoryPath();
    static QString backgroundCachePath();
    static QString fallbackBackgroundResourcePath();
};

#endif // APPCONFIG_H
