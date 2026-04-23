#include "logger.h"

#include <QDateTime>
#include <QDir>
#include <QDebug>
#include <QFileInfo>
#include <QMutexLocker>
#include <QTextStream>

Logger &Logger::instance()
{
    static Logger logger;
    return logger;
}

void Logger::initialize(const QString &logFilePath)
{
    QMutexLocker locker(&m_mutex);

    if (m_file.isOpen() && m_file.fileName() == logFilePath) {
        return;
    }

    if (m_file.isOpen()) {
        m_file.close();
    }

    QDir directory;
    directory.mkpath(QFileInfo(logFilePath).absolutePath());

    m_file.setFileName(logFilePath);
    m_ready = m_file.open(QIODevice::WriteOnly | QIODevice::Append | QIODevice::Text);

    if (!m_ready) {
        qWarning() << "No se pudo abrir el archivo de log:" << logFilePath;
        return;
    }

    writeLine(QString("Logger inicializado en %1").arg(logFilePath));
}

void Logger::log(const QString &message)
{
    QMutexLocker locker(&m_mutex);
    if (!m_ready) {
        qWarning() << message;
        return;
    }

    writeLine(message);
}

QString Logger::filePath() const
{
    return m_file.fileName();
}

void Logger::writeLine(const QString &message)
{
    QTextStream stream(&m_file);
    stream << '[' << QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss") << "] "
           << message << '\n';
    stream.flush();
    m_file.flush();
}
