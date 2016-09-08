#ifndef LOGGING_H
#define LOGGING_H

#include <QDebug>

#ifdef QT_DEBUG // debug target, full log output with source filename and line included
#define LOG_DEBUG(x)    qDebug().nospace() << "D " << __FILE__ << ":" << __LINE__ << " " << x
#define LOG_INFO(x)     qDebug().nospace() << "I " << __FILE__ << ":" << __LINE__ << " " << x
#define LOG_WARN(x)     qDebug().nospace() << "W!" << __FILE__ << ":" << __LINE__ << " " << x
#define LOG_CRITICAL(x) qDebug().nospace() << "C!" << __FILE__ << ":" << __LINE__ << " " << x
#else           // release target, logging reduced to necessary messages
#define LOG_DEBUG(x)
#define LOG_INFO(x)     qDebug() << x
#define LOG_WARN(x)     qDebug() << "Warning: " << x
#define LOG_CRITICAL(x) qDebug() << "Critical:" << x
#endif

#endif // LOGGING_H
