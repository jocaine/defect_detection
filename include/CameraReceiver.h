#ifndef CAMERARECEIVER_H
#define CAMERARECEIVER_H

#include "AbstractReceiver.h"
#include <opencv2/videoio.hpp>
#include <atomic>

class CameraReceiver : public AbstractReceiver {
    Q_OBJECT
public:
    explicit CameraReceiver(QObject* parent = nullptr);
    virtual ~CameraReceiver();

    bool pause() override;
    void read() override;

private:
    cv::VideoCapture m_cap;
    int m_deviceId = -1;
    std::atomic<bool> m_isRunning{false};
    std::atomic<bool> m_isPaused{false};
};

#endif // CAMERARECEIVER_H
