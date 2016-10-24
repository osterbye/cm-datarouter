#include <QCoreApplication>
#include "datarouter.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    Datarouter datarouter(&a);
    return a.exec();
}
