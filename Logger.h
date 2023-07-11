//
// Created by Dima on 19.03.2021.
//

#ifndef ASTRA_LOGGER_H
#define ASTRA_LOGGER_H

#include <iostream>

#include <QCoreApplication>
#include <QFile>
#include <QDateTime>
#include <QDebug>
#include <QDir>
#include <QDirIterator>
#include <QLoggingCategory>
#include <QTextStream>

#include "Config.h"
#include "macroses.h"

Q_DECLARE_LOGGING_CATEGORY(logDebug)
Q_DECLARE_LOGGING_CATEGORY(logInfo)
Q_DECLARE_LOGGING_CATEGORY(logWarning)
Q_DECLARE_LOGGING_CATEGORY(logCritical)
Q_DECLARE_LOGGING_CATEGORY(logFatal)

void log_init();
void log_delete();
void LogMessageOutput([[maybe_unused]] QtMsgType type, const QMessageLogContext &, const QString & str);
void LogFileOpening(int change);

//#define LOG_STDOUT

//#ifdef LOG_STDOUT
//    #define LOG(...) std::cout << __VA_ARGS__ << std::endl
//#elif LOG_PRINTF
//    #define LOG(...) printf(__VA_ARGS__)
//#elif defined LOG_QT
//    #define LOG(...)
//#else
//    #define LOG(...)
//#endif
#endif //ASTRA_LOGGER_H
