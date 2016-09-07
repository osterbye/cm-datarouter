#include <QtTest>

#include "pbmessenger_tst.h"
#include "statewriter_tst.h"

int main(int argc, char** argv)
{
    QCoreApplication a(argc, argv); // apparently needed for sql not to segfault during cleanup
    int status = 0;
    {
        PBMessenger_Tst pbMessenger_Tst;
        status |= QTest::qExec(&pbMessenger_Tst, argc, argv);
    }
    {
        StateWriter_Tst stateWriter_Tst;
        status |= QTest::qExec(&stateWriter_Tst, argc, argv);
    }
    google::protobuf::ShutdownProtobufLibrary();

    return status;
}
