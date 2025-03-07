#ifndef DATAPACKET_H
#define DATAPACKET_H
#include<cstdint>
#include<opencv4/opencv2/opencv.hpp>
#include<ctime>
struct DataPacket {
	uint16_t header;       // 包头 (0xAA55)
	uint8_t device_id;     // 设备ID
	float value;           // 数据值
	uint32_t timestamp;    // 时间戳
};
struct box {
    unsigned int x, y, w, h;       // (x,y) - top-left corner, (w, h) - width & height of bounded box
    float prob;                    // confidence - probability that the object was found correctly
    unsigned int obj_id;           // class of object - from range [0, classes-1]
    box(){}
    box(unsigned int x_,unsigned int y_,unsigned int w_,unsigned int h_,float prob_,unsigned int obj_id_)
        :x(x_)
        ,y(y_)
        ,w(w_)
        ,h(h_)
        ,prob(prob_)
        ,obj_id(obj_id_)
    {}
};
struct Mat_Packet
{
	uint8_t id;
	cv::Mat mat;
    time_t timestamp;
    Mat_Packet(cv::Mat m,uint8_t num)
        :mat(m)
        ,id(num)
    {
        time(&timestamp);
    }
    Mat_Packet(){}
};
struct result_Packet
{
    uint8_t id;
    time_t timestamp;
    std::vector<box> boxes;
    result_Packet(uint8_t id_,time_t t)
        :id(id_)
        ,timestamp(t)
        {}
    result_Packet()
    {}
    void push(box b)
    {
        boxes.push_back(b);
    }

};
#endif // !DATAPACKET_H
