#ifndef APPCONFIG_H
#define APPCONFIG_H

#include "models.h"

class AppConfigLoader
{
public:
    static AppConfig load();
    static QString configFilePath();
    static QString projectRootPath();
    static QString logsDirectoryPath();
    static QString defaultExportDirectoryPath();
};

#endif // APPCONFIG_H
