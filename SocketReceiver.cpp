#include "SocketReceiver.h"
#include <QDataStream>

SocketReceiver::SocketReceiver(const QHostAddress& host, quint16 port, QObject* parent)
    : AbstractReceiver(parent), m_host(host), m_port(port)
{
    m_socket = new QTcpSocket(this);
    connect(m_socket, &QTcpSocket::readyRead, this, &SocketReceiver::handleReadyRead);
    connect(m_socket, QOverload<QAbstractSocket::SocketError>::of(&QAbstractSocket::errorOccurred),this, &SocketReceiver::handleError);
}

bool SocketReceiver::start()
{
    m_socket->connectToHost(m_host, m_port);
    return m_socket->waitForConnected(3000);
}

bool SocketReceiver::pause()
{
    // 暂停逻辑可根据需要实现
    return true;
}

void SocketReceiver::read()
{
    // 通过Qt的信号槽机制自动触发数据读取
}

void SocketReceiver::handleReadyRead()
{
    QDataStream in(m_socket);
    in.setVersion(QDataStream::Qt_5_15);

    while(true)
    {
        if(m_expectedSize < 0)
        {
            if(m_socket->bytesAvailable() < sizeof(qint32))
            {
                return;
            }
            in >> m_expectedSize;
        }

        if(m_socket->bytesAvailable() < m_expectedSize)
        {
            return;
        }

        QByteArray packet;
        in >> packet;
        parseData(packet);
        m_expectedSize = -1;
    }
}

// SocketReceiver.cpp
void SocketReceiver::parseData(const QByteArray& data)
{
    QDataStream stream(data);
    stream.setVersion(QDataStream::Qt_5_15);

    uint8_t id;
    qint64 timestamp_qint64;  // 使用qint64替代time_t
    QByteArray imgData;

    stream >> id >> timestamp_qint64 >> imgData;

    // 转换为time_t类型
    time_t timestamp = static_cast<time_t>(timestamp_qint64);

}

void SocketReceiver::handleError(QAbstractSocket::SocketError error)
{
    qWarning() << "Socket error:" << m_socket->errorString();
    // 可根据需要实现重连逻辑
}

// 发送端应该这样序列化数据：
//QByteArray data;
//QDataStream stream(&data, QIODevice::WriteOnly);
//stream.setVersion(QDataStream::Qt_5_15);
//
//time_t timestamp = time(nullptr);
//stream << id << static_cast<qint64>(timestamp) << imgData;
