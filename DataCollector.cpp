#include "DataCollector.h"
#include<sys/shm.h>
#include<unistd.h>
#include<pthread.h>
#include"DataHandler.h"
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
void DataReciver::read(DataHandler* buff)
{
}
inline bool DataReciver::start() {}
void DataReciver::show()
{
}
DataReciver::~DataReciver() {}




IPC_Reciver::IPC_Reciver()
{
}

IPC_Reciver::~IPC_Reciver()
{
}

bool IPC_Reciver::start()
{
    //int shmid = shmget(SHM_KEY, sizeof(Share_Memory), IPC_CREAT | 0664);
    //if (shmid == -1)
    //{
    //	perror("shmget error!!!");
    //	return 0;
    //}
    //this->shm_ptr = shmat(shmid, NULL, 0);
    //if (shm_ptr == (void*)-1)
    //{
    //	perror("shmat error!!!");
    //	return 0;
    //}
    //return 1;
}

void IPC_Reciver::read(DataHandler* buff)
{
    //auto ptr = (Share_Memory*)shm_ptr;
    //while (1)
    //{
    //	if (ptr->capacity == SHM_SIZE)
    //	{
    //		usleep(2000);
    //		std::cout << "read:休息一下~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << std::endl;
    //	}
    //	pthread_mutex_lock(&ptr->mutex);
    //	DataPacket temp = ptr->DataList[ptr->temp];
    //	ptr->temp = (ptr->temp + 1) % ptr->tail;
    //	ptr->capacity++;
    //	pthread_mutex_unlock(&ptr->mutex);
    //	buff->input(temp);
    //}
	
}

void IPC_Reciver::show()
{
    //auto ptr = (Share_Memory*)shm_ptr;
    //if (ptr->capacity == SHM_SIZE)
    //{
    //	usleep(2000);
    //	std::cout << "show:休息一下~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << std::endl;
    //}
    //pthread_mutex_lock(&ptr->mutex);
    //DataPacket temp = ptr->DataList[ptr->temp];
    //ptr->temp = (ptr->temp + 1) % ptr->tail;
    //ptr->capacity++;
    //pthread_mutex_unlock(&ptr->mutex);
    //std::cout << "\n" << temp.device_id << "\n" << temp.header << "\n" << temp.timestamp << "\n" << temp.value << std::endl;
}








DataCollector::DataCollector(enum1 reciver_type)
{
	switch (reciver_type)
	{
	case IPC:
		reciver = new IPC_Reciver;
		break;
	case FS:
		reciver = new FS_Reciver;
		break;
	default:
		break;
	}
}
DataCollector::~DataCollector()
{
	delete reciver;
	delete handler;
}
void DataCollector::start()
{
	if(!reciver->start())return;
	if (handler)
    {
        handler->start();
        reciver->read(handler);
    }
	else
		reciver->show();
}

uint8_t DataCollector::serial_num(0);
void DataCollector::get_handler(DataHandler* h)
{
	handler = h;
}

Mat_Packet DataCollector::pack(std::filesystem::path f)
{
    if(!std::filesystem::is_regular_file(f))
        return Mat_Packet();
    return Mat_Packet(cv::imread(f.string()),DataCollector::get_id());
}

uint8_t DataCollector::get_id()
{
    return DataCollector::serial_num++;
}





FS_Reciver::FS_Reciver()
{
}

bool FS_Reciver::start()
{
    std::string path_str="/home/tlr/files";
    //std::cout << "请文件路径：";
    //std::cin >> path_str;
	if (path_str.empty())
	{
		std::cout << "vaild input!!!";
		return false;
	}
	if (!std::filesystem::is_directory(path_str))
	{
		std::cout << "vaild directory!!!";
		return false;
	}
	dir_path=path_str;
	return true;
}

void FS_Reciver::read(DataHandler* buff)
{
    for(const auto& file_name:std::filesystem::recursive_directory_iterator(this->dir_path))
    {
        std::cout<<file_name<<std::endl;
        buff->input(DataCollector::pack(file_name));
    }
}



