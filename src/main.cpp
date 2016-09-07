#include <QCoreApplication>
#include <QtCore>
#include <QDebug>
#include "pbmessenger.h"
#include "mockbus.h"
#include "statewriter.h"

class Task : public QObject
{
    Q_OBJECT

public:
    Task(QObject *parent = 0) : QObject(parent), busDevice(this), messenger(this)
    {
        QObject::connect(&busDevice, &MockBus::newMessageReceived, &messenger, &PBMessenger::receiveMessage);
        //QObject::connect(busDevice, &SpiBus::readyRead, &PBMessager.receiveInput);
        QObject::connect(&messenger, &PBMessenger::newStatusReceived, &stateWriter, &StateWriter::receiveStatus);
        //QObject::connect(messenger, &PBMessenger::newStatusUpdate, &calc.receiveStatus);
    }
    ~Task()
    {
        google::protobuf::ShutdownProtobufLibrary();
    }

private:
    MockBus busDevice; // TODO: once SpiBus is implemented, move MockBus to unit test project
    //SpiBus busDevice;
    PBMessenger messenger;
    StateWriter stateWriter;

public slots:
    void runTask()
    {
        qDebug() << "loop iteration";
        busDevice.readBus();
        //emit finished();
    }

signals:
    void finished();
};

/* TODO:
 *  SPI handler
 *  receiving commands from CMD engine (dbus)
 *  forwarding to Calc/checker? (dbus)
 */

#include "main.moc"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    // Task parented to the application so that it will be deleted by the application.
    Task *task = new Task(&a);

    // This will cause the application to exit when the task signals finished.
    QObject::connect(task, &Task::finished, &QCoreApplication::quit);

    QTimer taskTimer;
    // Run task periodically
    QObject::connect(&taskTimer, &QTimer::timeout, task, &Task::runTask);
    taskTimer.start(500);
    return a.exec();
}
