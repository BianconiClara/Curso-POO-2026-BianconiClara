#ifndef MODELS_H
#define MODELS_H

#include <QString>

struct AppConfig
{
    QString configPath;
    QString projectRootPath;
    QString initialUser = "admin";
    QString initialPassword = "1234";
    int lockSeconds = 15;
    QString defaultLanguage = "C++";
    QString exportDirectory;
    QString logFilePath;
};

struct ValidationResult
{
    bool valid = true;
    QString message = "Linea valida.";
};

#endif // MODELS_H
