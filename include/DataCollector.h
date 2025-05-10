#ifndef _DATACOLLECTOR_H
#define _DATACOLLECTOR_H
#include"DataPacket.h"
#include"AbstractReceiver.h"

#include<iostream>
#include<signal.h>
#include<QObject>
#include<string>
#include<opencv4/opencv2/opencv.hpp>
enum enum1
{
    Camera = 0,
    FS = 1,
    Socket=2
};

class DataCollector:public QObject
{
    Q_OBJECT
public:

    DataCollector(enum1 reciver_type,QObject* parent=nullptr);
	~DataCollector();

	void start();
    void pause();

signals:
    void MatPackage(Mat_Packet);

private:
    AbstractReceiver* receiver;

};

#endif // !DATACOLLECTOR_H
