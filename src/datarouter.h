#ifndef DATAROUTER_H
#define DATAROUTER_H

#include <QObject>
#include "mockbus.h"
#include "spibus.h"
#include "pbmessenger.h"
#include "statewriter.h"

class Datarouter : public QObject
{
    Q_OBJECT
public:
    explicit Datarouter(QObject *parent = 0);
    ~Datarouter();
private:
    SpiBus m_busDevice; // TODO: once SpiBus is implemented, move MockBus to unit test project
    //SpiBus m_busDevice;
    PBMessenger m_messenger;
    StateWriter m_stateWriter;
signals:

public slots:
};

#endif // DATAROUTER_H
