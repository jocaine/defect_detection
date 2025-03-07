#include "DataHandler.h"
Detector detector("../../darknet_config/dd.cfg","../../darknet_config/dd.weights");
std::mutex detect_mtx;
DataHandler::DataHandler()
    :pool(nullptr)
{

}
DataHandler::~DataHandler()
{
    delete pool;
}

void DataHandler::start()
{
    pool=new ThreadPool_SingleFunc<result_Packet,Mat_Packet>(Handler,2,6);
}
void DataHandler::input(Mat_Packet&& data)
{
    pool->add_Task(std::move(data));
}

result_Packet DataHandler::output()
{
    return this->pool->result_queue->pop();
}

void DataHandler::wait()
{
    std::cout<<"wait"<<std::endl;
    this->pool->result_lck.lock();
    this->pool->result_cond.wait(this->pool->result_lck);
    this->pool->result_lck.unlock();
}


result_Packet DataHandler::Handler(Mat_Packet data)
{
    if(data.mat.empty())
    return result_Packet();
    cv::Mat result_image =data.mat.clone();
    std::shared_ptr<image_t> darknet_image_ptr = Detector::mat_to_image(result_image);
    detect_mtx.lock();
    std::vector<bbox_t> detections = detector.detect(*darknet_image_ptr);
    detect_mtx.unlock();
    result_Packet result(data.id,data.timestamp);
    for (const auto& detection : detections) {
        cv::Scalar color;
        switch(detection.obj_id)
        {
        case 0:
            color=cv::Scalar(255, 165, 0);
            break;
        case 1:
            color=cv::Scalar(0,255,0);
            break;
        case 2:
            color=cv::Scalar(255, 0, 0);
            break;
        case 3:
            color=cv::Scalar(0, 0, 255);
            break;
        case 4:
            color=cv::Scalar(255, 255, 0);
            break;
        case 5:
            color=cv::Scalar(128, 0, 128);
            break;

        }
        //绘制缺陷框
        cv::rectangle(result_image,
                      cv::Rect(detection.x, detection.y, detection.w, detection.h),
                      color);

        std::string label = "Class: " + std::to_string(detection.obj_id);
        cv::putText(result_image, label,
                    cv::Point(detection.x, detection.y + 10),
                    cv::FONT_HERSHEY_SIMPLEX, 0.3, color, 1);
        //传递
        box b(detection.x,detection.y,detection.w,detection.h,detection.prob,detection.obj_id);
        result.push(std::move(b));
    }

    if(detections.size()>0)
    {
        std::string filename("../../output/");
        filename+=std::to_string(pthread_self());
        filename+=std::to_string(data.id);
        filename+=".jpg";
        cv::imwrite(filename,result_image);
    }
    return result;
}
