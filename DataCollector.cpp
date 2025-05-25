#include "DataCollector.h"

#include"FSReceiver.h"
#include"CameraReceiver.h"
#include"SocketReceiver.h"


#include<QVBoxLayout>
#include<QHBoxLayout>

#include<sys/shm.h>
#include<unistd.h>
#include<pthread.h>

#define SHM_SIZE 10
#define SHM_KEY 100
//struct Share_Memory
//{
//	pthread_mutex_t mutex;
//	size_t capacity;
//	size_t temp;
//	size_t head;
//	size_t tail;
//	DataPacket DataList[SHM_SIZE];
//};

DataCollector::DataCollector(QWidget* parent)
    :QWidget(parent)
    ,receiver(nullptr)
    ,btnStart(nullptr)
{
    auto layout = new QVBoxLayout(this);
    auto buttom=new QHBoxLayout(this);

    auto cameraButton = new QPushButton("Camera Receiver", this);
    auto fsButton = new QPushButton("File System Receiver", this);
    auto socketButton = new QPushButton("Socket Receiver", this);

    auto  btnPath=new QPushButton("select output path", this);
    btnStart=new QPushButton("start collect", this);
    btnStart->setEnabled(false);

    buttom->addWidget(btnPath);
    buttom->addWidget(btnStart);

    // 设置按钮样式示例
    QString buttonStyle = "QPushButton { padding: 10px; font-size: 14px; }";
    cameraButton->setStyleSheet(buttonStyle);
    fsButton->setStyleSheet(buttonStyle);
    btnPath->setStyleSheet(buttonStyle);
    btnPath->setStyleSheet(buttonStyle);
    btnStart->setStyleSheet(buttonStyle);

    layout->addWidget(cameraButton);
    layout->addWidget(fsButton);
    layout->addWidget(socketButton);
    layout->addLayout(buttom);

    // 连接按钮信号
    connect(cameraButton, &QPushButton::clicked, this, [=](){
        initReceiver(Camera);});
    connect(fsButton, &QPushButton::clicked, this, [=](){
        initReceiver(FS);});
    connect(socketButton, &QPushButton::clicked, this, [=](){
        initReceiver(Socket);});
    connect(btnStart, &QPushButton::clicked, this, [=](){
        receiver->read();
        emit sgStartReading();
    });


}

DataCollector::~DataCollector()
{
    delete receiver;
}

void DataCollector::pause()
{

}

void DataCollector::initReceiver(enum1 type)
{
    if (receiver) {
        delete receiver;
        receiver = nullptr;
    }


    switch (type) {
    case Camera:
        receiver = new CameraReceiver;
        break;
    case FS:
        receiver = new FS_Receiver();
        break;
    case Socket:
        receiver = new SocketReceiver();
        break;
    }

    if (receiver)
    {
    btnStart->setEnabled(true);
    connect(receiver, &AbstractReceiver::MatPackage, this, &DataCollector::MatPackage);
    }
}





