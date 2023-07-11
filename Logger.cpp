//
// Created by Dima on 19.03.2021.
//

#include "Logger.h"

Q_LOGGING_CATEGORY(logDebug,    "Debug")
Q_LOGGING_CATEGORY(logInfo,     "Info")
Q_LOGGING_CATEGORY(logWarning,  "Warning")
Q_LOGGING_CATEGORY(logCritical, "Critical")
Q_LOGGING_CATEGORY(logFatal,    "Fatal")

QScopedPointer<QFile> logFile;

void log_init() {

    log_delete();

    LogFileOpening(1);

    switch (cfg.LOG_LEVEL) {
        case LOG_LEV::DEBUG:
            QLoggingCategory::setFilterRules("*.debug=false\n"
                                             "Debug.debug=true\n"
                                             "Info.debug=true\n"
                                             "Warning.debug=true\n"
                                             "Critical.debug=true\n"
                                             "Fatal.debug=true\n");
            break;
        case LOG_LEV::INFO:
            QLoggingCategory::setFilterRules("*.debug=false\n"
                                             "Info.debug=true\n"
                                             "Warning.debug=true\n"
                                             "Critical.debug=true\n"
                                             "Fatal.debug=true\n");
            break;
        case LOG_LEV::WARNING:
            QLoggingCategory::setFilterRules("*.debug=false\n"
                                             "Warning.debug=true\n"
                                             "Critical.debug=true\n"
                                             "Fatal.debug=true\n");
            break;
        case LOG_LEV::CRITICAL:
            QLoggingCategory::setFilterRules("*.debug=false\n"
                                             "Critical.debug=true\n"
                                             "Fatal.debug=true\n");
            break;
        case LOG_LEV::FATAL:
            QLoggingCategory::setFilterRules("*.debug=false\n"
                                             "Fatal.debug=true\n");
            break;
        case LOG_LEV::NONE_LOG:
            QLoggingCategory::setFilterRules("*.debug=false\n");
            break;
    }

    if (cfg.LOG_TO_FILE) {
        qInstallMessageHandler(LogMessageOutput);
    }

//    qCDebug(logDebug()) << "CONFIG" << '\n'
//                        << AS_KV(cfg.PORT) << AS_KV(cfg.CHANNELS)
//                        << AS_KV(cfg.ORGANIZATION_NAME) << AS_KV(cfg.APPLICATION_NAME);
}

void log_delete() {

    const QDate today = QDate::currentDate();
    const QString path = QCoreApplication::applicationDirPath() + "/log/";

    QDir dir = QDir(path);
    if (!dir.exists()) {
        dir.mkpath(".");
    } else {
        QDirIterator it(path, QStringList() << "*.txt", QDir::Files, QDirIterator::Subdirectories);
        while (it.hasNext()) {
            QString name = it.next();
            //qCDebug(logDebug()) << name;
            QFileInfo fileInfo = it.fileInfo();

            //qCDebug(logDebug()) << fileInfo.lastModified().date().daysTo(today);
            if (fileInfo.lastModified().date().daysTo(today) > cfg.LOG_KEEP_DAYS) {
                QString filepath = fileInfo.absoluteFilePath();
                QDir deletefile;
                deletefile.setPath(filepath);
                deletefile.remove(filepath);

                //qCDebug(logDebug()) << "File " + filepath + " is deleted!";
            }

            QDir filedir = fileInfo.dir();
            if (filedir.isEmpty()) {
                //qCDebug(logDebug()) << "Dir " + fileInfo.dir().path() + " is deleted!";
                filedir.removeRecursively();
            }
        }
    }
}

void LogMessageOutput(QtMsgType type, const QMessageLogContext &context, const QString & msg)
{
    LogFileOpening(0);
    UNUSED(type);
    QString dt = QDateTime::currentDateTime().toString(DT_FORMAT);
    QTextStream out(logFile.data());
    out << dt << " [" << context.category << "] " << " : " << msg << "\n";
    out.flush();
    std::cout << dt.toStdString() << " [" << context.category << "] " << " : " << msg.toStdString() << std::endl;
}

void LogFileOpening(int change) {
    QString DATETIME_SUFFIX = "";
    //qCDebug(logDebug()) << cfg.FILE;
    if (cfg.LOG_DATE) {
        if (!logFile.isNull()) {
            QString filename = logFile->fileName();
            QStringList filename_parts = filename.split("_");
            if (filename_parts.length() > 1) {
                if (filename_parts.at(1) != QDate::currentDate().toString("yyyyMMdd")) {
                    change = 1;
                }
            }
//            if (filename_parts.length() > 2) {
//                if (filename_parts.at(2) != QTime::currentTime().toString("hhmm")) {
//                    change = 1;
//                }
//            }
        }
        DATETIME_SUFFIX = QDate::currentDate().toString("_yyyyMMdd"); // + QTime::currentTime().toString("_hhmm");
    }

    if (change == 1) {
        const QString path = QCoreApplication::applicationDirPath() + "/log/";

        QDir dir = QDir(path);
        if (!dir.exists()) {
            dir.mkpath(".");
        }

        logFile.reset(new QFile(qApp->applicationDirPath() + "/log/" + cfg.FILE.c_str() + DATETIME_SUFFIX + ".txt"));
        logFile.data()->open(QFile::Append | QFile::Text);
    }
}
