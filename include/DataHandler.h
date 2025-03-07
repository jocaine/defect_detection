#pragma once
#include<iostream>
#include"ThreadPool_SingleFunc.hpp"
#include"DataPacket.h"
#include"yolo_v2_class.hpp"
class DataHandler
{
public:
	DataHandler();
	~DataHandler();
    void start();
    void input(Mat_Packet&& data);
    result_Packet output();
    void wait();
private:
    static result_Packet Handler(Mat_Packet);
private:
    ThreadPool_SingleFunc<result_Packet,Mat_Packet>* pool;
	
};

