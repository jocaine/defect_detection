#ifndef SOCKETRECEIVER_H
#define SOCKETRECEIVER_H

#include "AbstractReceiver.h"
#include <QTcpSocket>
#include<QTcpServer>

class SocketReceiver : public AbstractReceiver {
    Q_OBJECT
public:
    explicit SocketReceiver(QObject* parent = nullptr);
    virtual ~SocketReceiver();

    bool pause() override;
    void read() override;

private slots:
    void handleReadyRead();
    void handleError(QAbstractSocket::SocketError error);

private:
    void parseData(const QByteArray& data);

    QTcpSocket* m_socket;
    QHostAddress m_host;
    quint16 m_port;
    qint32 m_expectedSize = -1;
    std::atomic<bool> m_isConnected{false};
};

#endif // SOCKETRECEIVER_H
