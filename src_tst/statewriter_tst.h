#ifndef STATEWRITER_TST_H
#define STATEWRITER_TST_H

#include <QObject>
#define private public // to access private members - for unit-testing only!
#include "src/statewriter.h"
#undef private

class StateWriter_Tst: public QObject
{
    Q_OBJECT

public:
    StateWriter_Tst(QObject *parent = 0) : QObject(parent)
    {}
private slots:
    // actual tests
    void openDatabase();
    void receiveUpdate();

private:
    StateWriter * m_statewriter;
};

#endif // STATEWRITER_TST_H
