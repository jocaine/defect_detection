#pragma once
#include"DataPacket.h"
#include"ThreadPool_SingleFunc.hpp"
#include"yolo_v2_class.hpp"

#include<QObject>
#include<iostream>
class DataHandler:public QObject
{
    Q_OBJECT
public:
    DataHandler(QObject* parent=nullptr);
	~DataHandler();
    void start();
    void input(Mat_Packet&& data);
    result_Packet output();
    void wait();

public slots:
    void slInput(Mat_Packet);

private:
    static result_Packet Handler(Mat_Packet);

private:
    ThreadPool_SingleFunc<result_Packet,Mat_Packet>* pool;
	
};

