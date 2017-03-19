#ifndef CAMERAHANDLER_H
#define CAMERAHANDLER_H

#include <QObject>

class QProcess;

class CameraHandler : public QObject
{
    Q_OBJECT
public:
    explicit CameraHandler(QObject *parent = 0);

public slots:
    void startStreaming();

private:
    struct usbCam{
        QString device;
        int width;
        int height;
        int framerate;
        usbCam(QString d, int w, int h, int r) {
            device = d;
            width = w;
            height = h;
            framerate = r;
        }
    };

    usbCam *detectCamera();
    void startCameraStreaming(usbCam *cam, QString host, quint16 port);

private:
    QProcess *m_streamProcess;
    QString m_host;
    quint16 m_port;
    bool m_isStreaming;
    usbCam *m_camera;
};

#endif // CAMERAHANDLER_H
