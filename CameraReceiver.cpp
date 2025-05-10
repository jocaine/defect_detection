#include "CameraReceiver.h"
#include <QThread>
#include <chrono>

CameraReceiver::CameraReceiver(int deviceId, QObject* parent)
    : AbstractReceiver(parent), m_deviceId(deviceId)
{
}

CameraReceiver::~CameraReceiver()
{
    if (m_cap.isOpened())
        m_cap.release();
}

bool CameraReceiver::start()
{
    if (!m_cap.open(m_deviceId)) {
        qWarning("Failed to open camera device %d", m_deviceId);
        return false;
    }

    // 设置摄像头参数（可选）
    m_cap.set(cv::CAP_PROP_FRAME_WIDTH, 640);
    m_cap.set(cv::CAP_PROP_FRAME_HEIGHT, 480);
    m_cap.set(cv::CAP_PROP_FPS, 30);

    m_isRunning.store(true);
    return true;
}

bool CameraReceiver::pause()
{
    m_isPaused.store(!m_isPaused.load());
    return true;
}

void CameraReceiver::read()
{
    cv::Mat frame;
    while (m_isRunning.load()) {
        if (m_isPaused.load()) {
            QThread::msleep(100);
            continue;
        }

        if (!m_cap.read(frame) || frame.empty()) {
            qWarning("Failed to read frame from camera");
            QThread::msleep(100);
            continue;
        }

        // OpenCV默认使用BGR格式，转换为RGB
        cv::cvtColor(frame, frame, cv::COLOR_BGR2RGB);

        // 生成时间戳
        time_t timestamp;
        time(&timestamp);

        // 创建数据包
        Mat_Packet packet(frame, UniqueID::getNext());
        packet.timestamp = timestamp;

        // 发射信号
        emit MatPackage(packet);

        // 控制帧率（约30fps）
        QThread::msleep(33);
    }
    m_cap.release();
}
