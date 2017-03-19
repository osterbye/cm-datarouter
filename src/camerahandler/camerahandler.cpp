#include "camerahandler.h"
#include <QCameraInfo>
#include <QProcess>

//#include <QDebug>

CameraHandler::CameraHandler(QObject *parent) : QObject(parent)
{
    m_host = "master.spiri.drivr.com";
    m_port = 8101;
    m_isStreaming = false;

    m_camera = detectCamera();
    if (0 == m_camera->framerate) {
        qDebug("FAILED TO DETECT USB CAMERA!");
    }
}

void CameraHandler::startStreaming()
{
    if (!m_isStreaming && (m_camera->framerate > 0)) {
        m_isStreaming = true;
        startCameraStreaming(m_camera, m_host, m_port);
    }
}

/**
 * @brief CameraHandler::detectCamera
 * @return The first camera matching requirements
 */
CameraHandler::usbCam *CameraHandler::detectCamera()
{
    QList<CameraHandler::usbCam> cams;
    QList<QCameraInfo> cameras = QCameraInfo::availableCameras();
    foreach (const QCameraInfo &cameraInfo, cameras) {
        QString s = "v4l2-ctl --device=" + cameraInfo.deviceName() + " --list-framesizes=H264";
        QProcess pSize;
        pSize.start(s);
        pSize.waitForFinished();
        QStringList frameSizes = QString(pSize.readAllStandardOutput()).split(" ");
        //qDebug() << "list:\n" << frameSizes;
        /*
          Assumes the following output format:
            ioctl: VIDIOC_ENUM_FRAMESIZES
                Size: Discrete 1280x720
                Size: Discrete 800x600
                Size: Discrete 640x480
                Size: Discrete 640x360
                Size: Discrete 1280x720
         */
        foreach (const QString &frameSize, frameSizes) {
            // For now, only look for 640x480 cams
            int resWidth = 640;
            int resHeight = 480;
            if (frameSize.startsWith("640x480")) {
                QString rateCmd = "v4l2-ctl --device=" + cameraInfo.deviceName() + " --list-frameintervals=width=" + QString::number(resWidth) + ",height=" + QString::number(resHeight) + ",pixelformat=H264";
                //qDebug() << rateCmd;
                QProcess pRate;
                pRate.start(rateCmd);
                pRate.waitForFinished();
                QStringList rateList = QString(pRate.readAllStandardOutput()).split("(");
                //qDebug() << rateList;
                /*
                 Assumes the following format:
                    ioctl: VIDIOC_ENUM_FRAMEINTERVALS
                        Interval: Discrete 0.033s (30.000 fps)
                        Interval: Discrete 0.050s (20.000 fps)
                        Interval: Discrete 0.067s (15.000 fps)
                        Interval: Discrete 0.100s (10.000 fps)
                        Interval: Discrete 0.200s (5.000 fps)
                */
                foreach (const QString &frameRate, rateList) {
                    QStringList r = frameRate.split(".");
                    bool ok = false;
                    int rate = QString(r.first()).toInt(&ok);
                    if (ok) {
                        cams.append(CameraHandler::usbCam(cameraInfo.deviceName(), resWidth, resHeight, rate));
                        if (15 == rate)
                            return &cams.last();
                    }
                }
            }
        }
    }
    if (!cams.isEmpty())
        return &cams.last();

    return new CameraHandler::usbCam("", 0, 0, 0);
}

void CameraHandler::startCameraStreaming(usbCam *cam, QString host, quint16 port)
{
    //qDebug() << "Starting streaming to " << host << ":" << QString::number(port) << " using " << cam.device << " at " << QString::number(cam.width) << "x" << QString::number(cam.height) << "@" << QString::number(cam.framerate);
    QString streamCmd = "gst-launch-1.0 -e v4l2src device=" + cam->device + " ! video/x-h264,width=" + QString::number(cam->width) + ",height=" + QString::number(cam->height) + ",framerate=" + QString::number(cam->framerate) + "/1,bitrate=512 ! rtph264pay config-interval=1 pt=96 ! udpsink host=" + host + " port=" + QString::number(port);
    //QString streamCmd = "gst-launch-1.0 -e v4l2src device=" + cam.device + " ! video/x-h264,width=" + QString::number(cam.width) + ",height=" + QString::number(cam.height) + ",framerate=" + QString::number(cam.framerate) + "/1,bitrate=512 ! h264parse ! mp4mux ! filesink location=/media/sdcard/handlertest01.mp4";
    //qDebug() << "Starting streaming:\n" << streamCmd;
    m_streamProcess = new QProcess();
    m_streamProcess->start(streamCmd);
    m_streamProcess->waitForStarted();
    //qDebug() << "error? = " << m_streamProcess->readAllStandardError() << ", stdout: " << m_streamProcess->readAllStandardOutput();
}
