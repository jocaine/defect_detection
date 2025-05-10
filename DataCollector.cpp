#include "DataCollector.h"

#include"FSReceiver.h"
#include"CameraReceiver.h"
#include"SocketReceiver.h"

#include<sys/shm.h>
#include<unistd.h>
#include<pthread.h>

#define SHM_SIZE 10
#define SHM_KEY 100
struct Share_Memory
{
	pthread_mutex_t mutex;
	size_t capacity;
	size_t temp;
	size_t head;
	size_t tail;
	DataPacket DataList[SHM_SIZE];
};

DataCollector::DataCollector(enum1 receiver_type,QObject* parent)
    :QObject(parent)
{
    switch (receiver_type)
	{
    case Camera:
        receiver = new CameraReceiver;
		break;
	case FS:
        receiver = new FS_Receiver();
        break;
    case Socket:
        receiver=new SocketReceiver();
        break;
	default:
		break;
	}
    if(receiver)
    {
    connect(receiver,&AbstractReceiver::MatPackage,this,&DataCollector::MatPackage);
    }
}

DataCollector::~DataCollector()
{
    delete receiver;
}

void DataCollector::start()
{
    if(!receiver->start())
    {
        return;
    }
    receiver->read();
}

void DataCollector::pause()
{

}






