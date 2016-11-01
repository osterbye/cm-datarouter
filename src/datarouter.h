#ifndef DATAROUTER_H
#define DATAROUTER_H

#include <QObject>
#include "mockbus.h"
#include "spibus.h"
#include "pbmessenger.h"
#include "statewriter.h"

class Pubnub_spiri;

class Datarouter : public QObject
{
    Q_OBJECT
public:
    explicit Datarouter(QObject *parent = 0);
    ~Datarouter();

signals:

public slots:

private:
    SpiBus m_busDevice; // TODO: once SpiBus is implemented, move MockBus to unit test project
    //SpiBus m_busDevice;
    PBMessenger m_messenger;
    StateWriter m_stateWriter;

    Pubnub_spiri *m_pubnubHandler;
};

#endif // DATAROUTER_H
