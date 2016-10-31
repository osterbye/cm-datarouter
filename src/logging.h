#ifndef LOGGING_H
#define LOGGING_H

#include <QDebug>

#ifdef QT_DEBUG // debug target, full log output with source filename and line included

#define LOG_DEBUG(x)    do { qDebug().nospace() << "D " << __FILE__ << ":" << __LINE__ << " " << x; } while(0)
#define LOG_INFO(x)     do { qDebug().nospace() << "I " << __FILE__ << ":" << __LINE__ << " " << x; } while(0)
#define LOG_WARN(x)     do { qDebug().nospace() << "W!" << __FILE__ << ":" << __LINE__ << " " << x; } while(0)
#define LOG_CRITICAL(x) do { qDebug().nospace() << "C!" << __FILE__ << ":" << __LINE__ << " " << x; } while(0)

#else           // release target, logging reduced to necessary messages

#define LOG_DEBUG(x)    do {} while(0)
#define LOG_INFO(x)     do {qDebug() << x;                } while(0)
#define LOG_WARN(x)     do {qDebug() << "Warning: " << x; } while(0)
#define LOG_CRITICAL(x) do {qDebug() << "Critical:" << x; } while(0)

#endif

#endif // LOGGING_H
