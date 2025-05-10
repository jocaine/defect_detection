#ifndef CAMERARECEIVER_H
#define CAMERARECEIVER_H

#include "AbstractReceiver.h"
#include <opencv2/videoio.hpp>
#include <atomic>
class CameraReceiver : public AbstractReceiver
{
    Q_OBJECT
public:
    explicit CameraReceiver(int deviceId = 0, QObject* parent = nullptr);
    virtual ~CameraReceiver();

    bool start() override;
    bool pause() override;
    void read() override;

private:
    cv::VideoCapture m_cap;
    int m_deviceId;
    std::atomic<bool> m_isRunning{false};
    std::atomic<bool> m_isPaused{false};
};

#endif // CAMERARECEIVER_H
