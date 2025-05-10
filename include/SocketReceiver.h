// SocketReceiver.h
#ifndef SOCKETRECEIVER_H
#define SOCKETRECEIVER_H

#include "AbstractReceiver.h"
#include <QTcpSocket>
#include <QHostAddress>

class SocketReceiver : public AbstractReceiver
{
    Q_OBJECT
public:
    SocketReceiver(const QHostAddress& host = QHostAddress::LocalHost,quint16 port = 12345, QObject* parent = nullptr);
    virtual bool start() override;
    virtual bool pause() override;
    virtual void read() override;

private slots:
    void handleReadyRead();
    void handleError(QAbstractSocket::SocketError error);

private:
    void parseData(const QByteArray& data);

    QTcpSocket* m_socket;
    QHostAddress m_host;
    quint16 m_port;
    qint32 m_expectedSize = -1;
};

#endif // SOCKETRECEIVER_H
