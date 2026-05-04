#ifndef LOGGER_H
#define LOGGER_H

#include <QFile>
#include <QMutex>
#include <QString>

class Logger
{
public:
    static Logger &instance();

    void initialize(const QString &logFilePath);
    void log(const QString &message);
    QString filePath() const;

private:
    Logger() = default;
    Logger(const Logger &) = delete;
    Logger &operator=(const Logger &) = delete;

    void writeLine(const QString &message);

    QFile m_file;
    QMutex m_mutex;
    bool m_ready = false;
};

#endif // LOGGER_H
