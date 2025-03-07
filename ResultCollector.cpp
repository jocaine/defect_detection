#include "ResultCollector.h"

ResultCollector::ResultCollector(DataHandler* h)
    :handler(h)
    ,total_num(0)
    ,crazing_num(0)
    ,inclusion_num(0)
    ,patches_num(0)
    ,pitted_surface_num(0)
    ,rolled_scale_num(0)
    ,scratches_num(0)
{
    pthread_create(&this->collector_thread,NULL,collect,this);
}

void *ResultCollector::collect(void *arg)
{
    auto rc=static_cast<ResultCollector*>(arg);

    while(1)
    {
        rc->handler->wait();

        auto result=rc->handler->output();
        rc->total_num.store(rc->total_num+1,std::memory_order_relaxed);
        std::cout<<"total"<<rc->total_num.load()<<std::endl;
        int cra=0;
        int in=0;
        int pa=0;
        int pi=0;
        int ro=0;
        int sc=0;
        for(auto box:result.boxes)
        {
            switch(box.obj_id)
            {
            case 0:
                cra++;
                if(cra==1)
                    rc->crazing_num.store(rc->crazing_num+1,std::memory_order_relaxed);
                break;
            case 1:
                in++;
                if(in==1)
                    rc->inclusion_num.store(rc->inclusion_num+1,std::memory_order_relaxed);
                break;
            case 2:
                pa++;
                if(pa==1)
                    rc->patches_num.store(rc->patches_num+11,std::memory_order_relaxed);
                break;
            case 3:
                pi++;
                if(pi==1)
                    rc->pitted_surface_num.store(rc->pitted_surface_num+1,std::memory_order_relaxed);
                break;
            case 4:
                ro++;
                if(ro==1)
                    rc->rolled_scale_num.store(rc->rolled_scale_num+1,std::memory_order_relaxed);
                break;
            case 5:
                sc++;
                if(sc==1)
                    rc->scratches_num.store(rc->scratches_num+1,std::memory_order_relaxed);
                break;
            }
        }

    }
}


