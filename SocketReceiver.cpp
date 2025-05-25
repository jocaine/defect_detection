#include "SocketReceiver.h"
#include "socketConfigDialog.h"
#include <QDataStream>

SocketReceiver::SocketReceiver(QObject* parent)
    : AbstractReceiver(parent),
    m_socket(new QTcpSocket(this))
{
    QHostAddress host;
    quint16 port;

    if (SocketConfigDialog::getConnectionParams(&host, &port)) {
        m_host = host;
        m_port = port;

        connect(m_socket, &QTcpSocket::connected, [this](){
            m_isConnected.store(true);
            emit sgReadyToRead(); // 连接成功触发就绪信号
        });

        connect(m_socket, &QTcpSocket::readyRead,
                this, &SocketReceiver::handleReadyRead);
        connect(m_socket, QOverload<QAbstractSocket::SocketError>::of(&QAbstractSocket::errorOccurred),
                this, &SocketReceiver::handleError);

        m_socket->connectToHost(m_host, m_port);
    }
}

SocketReceiver::~SocketReceiver()
{
    if (m_socket->state() == QAbstractSocket::ConnectedState) {
        m_socket->disconnectFromHost();
    }
}

bool SocketReceiver::pause()
{
    // 网络连接暂不支持暂停
    return true;
}

void SocketReceiver::read()
{
    // 数据通过异步的readyRead信号处理
}

void SocketReceiver::handleReadyRead()
{
    QDataStream in(m_socket);
    in.setVersion(QDataStream::Qt_5_15);

    while (m_socket->bytesAvailable() > 0) {
        if (m_expectedSize < 0) {
            if (m_socket->bytesAvailable() < sizeof(qint32)) return;
            in >> m_expectedSize;
        }

        if (m_socket->bytesAvailable() < m_expectedSize) return;

        QByteArray packet;
        in >> packet;
        parseData(packet);
        m_expectedSize = -1;
    }
}

void SocketReceiver::parseData(const QByteArray& data)
{
    QDataStream stream(data);
    stream.setVersion(QDataStream::Qt_5_15);

    uint8_t id;
    qint64 timestamp;
    QByteArray imgData;

    stream >> id >> timestamp >> imgData;

    cv::Mat frame = cv::imdecode(
        std::vector<uchar>(imgData.begin(), imgData.end()),
        cv::IMREAD_COLOR
        );

    if (!frame.empty()) {
        emit MatPackage(new Mat_Packet(frame, UniqueID::getNext()));
    }
}

void SocketReceiver::handleError(QAbstractSocket::SocketError error)
{
    qWarning() << "Socket error:" << m_socket->errorString();
    m_isConnected.store(false);
}
