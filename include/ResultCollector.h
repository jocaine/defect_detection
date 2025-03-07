#ifndef RESULTCOLLECTOR_H
#define RESULTCOLLECTOR_H
#include"DataHandler.h"
#include"QThread"
#include<atomic>
class ResultCollector
{
public:
    ResultCollector(DataHandler* h);
    DataHandler* handler;
    std::atomic<float> total_num;
    std::atomic<float> crazing_num;
    std::atomic<float> inclusion_num;
    std::atomic<float> patches_num;
    std::atomic<float> pitted_surface_num;
    std::atomic<float> rolled_scale_num;
    std::atomic<float> scratches_num;
private:
    void display(result_Packet&& packet);
    static void* collect(void* arg);
private:
    pthread_t collector_thread;
    QThread* thread;
};

#endif // RESULTCOLLECTOR_H
