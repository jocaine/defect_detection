#ifndef ABSTRACTRECEIVER_H
#define ABSTRACTRECEIVER_H
#include"DataPacket.h"
#include<QObject>
class AbstractReceiver:public QObject
{
    Q_OBJECT
public:
    AbstractReceiver(QObject* parent=nullptr);
    virtual ~AbstractReceiver()=0;

    virtual bool pause()=0;

    virtual void read()=0;

signals:
    void MatPackage(Mat_Packet*);
    void sgReadyToRead();
};

#endif // ABSTRACTRECEIVER_H
