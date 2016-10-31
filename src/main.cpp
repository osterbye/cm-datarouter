#include <QCoreApplication>
#include "datarouter.h"
#include "pubnub_spiri.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    //Datarouter datarouter(&a);
    Pubnub_spiri pubnub_spiri(&a);

    QString channel = "chn-test";
    QJsonObject jsonObj;
    jsonObj.insert("voltage", 123);
    jsonObj.insert("current", 1234);
    QJsonDocument message(jsonObj);

    pubnub_spiri.pnSendMessage(channel,message);

    return a.exec();
}
