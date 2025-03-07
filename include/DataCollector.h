#ifndef _DATACOLLECTOR_H
#define _DATACOLLECTOR_H
#include<iostream>
#include"DataPacket.h"
#include<signal.h>
#include"DataHandler.h"
#include<filesystem>
#include<string>
#include<opencv4/opencv2/opencv.hpp>
enum enum1
{
	IPC = 0,
	FS = 1
};
class DataReciver
{
public:
	DataReciver() {};
	virtual void read(DataHandler* buff);
	virtual bool start();
	virtual void show();
	virtual ~DataReciver()=0;
};
class IPC_Reciver :public DataReciver
{
public:
	IPC_Reciver();
	bool start()override;
	void read(DataHandler* buff)override;
	void show();
private:
	~IPC_Reciver()override;
	void* shm_ptr;
};
class FS_Reciver:public DataReciver
{
public:
	FS_Reciver();
	bool start()override;
	void read(DataHandler* buff)override;
private:
	std::filesystem::path dir_path;
};
class DataCollector
{
public:

	DataCollector(enum1 reciver_type);
	~DataCollector();
	void start();
	void get_handler(DataHandler* h);
    static Mat_Packet pack(std::filesystem::path path);
    static uint8_t get_id();
private:
	DataReciver* reciver;
	DataHandler* handler;
    static uint8_t serial_num;
};

#endif // !DATACOLLECTOR_H
