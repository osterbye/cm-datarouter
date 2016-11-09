#include <QCoreApplication>
#include "pubnubtest.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    PubNubTest pntest(&a);
    return a.exec();
}
