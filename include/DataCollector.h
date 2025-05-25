#ifndef _DATACOLLECTOR_H
#define _DATACOLLECTOR_H
#include"DataPacket.h"
#include"AbstractReceiver.h"

#include<QWidget>
#include<QPushButton>
#include<iostream>
#include<signal.h>
#include<string>
#include<opencv4/opencv2/opencv.hpp>

enum enum1
{
    Camera = 0,
    FS = 1,
    Socket=2
};

class DataCollector:public QWidget
{
    Q_OBJECT
public:

    DataCollector(QWidget* parent=nullptr);
	~DataCollector();

    void pause();

signals:
    void MatPackage(Mat_Packet*);
    void sgStartReading();

private slots:
    void initReceiver(enum1);

private:
    QPushButton* btnStart;
    AbstractReceiver* receiver;

};

#endif // !DATACOLLECTOR_H
