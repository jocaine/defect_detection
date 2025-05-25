#include "CameraReceiver.h"
#include "cameraConfigDialog.h"
#include <QThread>
#include <chrono>

CameraReceiver::CameraReceiver(QObject* parent)
    : AbstractReceiver(parent)
{
    bool ok;
    m_deviceId = CameraConfigDialog::getDeviceId(&ok);

    if (ok && m_cap.open(m_deviceId)) {
        m_cap.set(cv::CAP_PROP_FRAME_WIDTH, 640);
        m_cap.set(cv::CAP_PROP_FRAME_HEIGHT, 480);
        m_cap.set(cv::CAP_PROP_FPS, 30);
        emit sgReadyToRead(); // 触发就绪信号
    } else {
        qWarning("Failed to initialize camera");
    }
}

CameraReceiver::~CameraReceiver()
{
    m_isRunning.store(false);
    if (m_cap.isOpened())
        m_cap.release();
}

bool CameraReceiver::pause()
{
    m_isPaused.store(!m_isPaused.load());
    return true;
}

void CameraReceiver::read()
{
    m_isRunning.store(true);
    cv::Mat frame;

    while (m_isRunning.load()) {
        if (m_isPaused.load()) {
            QThread::msleep(100);
            continue;
        }

        if (!m_cap.read(frame) || frame.empty()) {
            qWarning("Camera read error");
            break;
        }

        cv::cvtColor(frame, frame, cv::COLOR_BGR2RGB);
        emit MatPackage(new Mat_Packet(frame.clone(), UniqueID::getNext()));
        QThread::msleep(33);
    }

    m_cap.release();
    m_isRunning.store(false);
}
