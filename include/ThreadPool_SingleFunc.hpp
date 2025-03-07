#ifndef _SIGLEFUNCTHREADPOOL_HPP
#define _SIGLEFUNCTHREADPOOL_HPP
#include"ThreadSafeQueue.hpp"
#include<pthread.h>
#include<mutex>
#include<condition_variable>
#include<iostream>
#include<string>
#include<string.h>
#include<unistd.h>
#include<functional>
#define IS_VOID 0
inline bool isVaild(void* arg)
{
	if (arg == nullptr)
	{
		std::cout << "指针检测为空" << std::endl;
		return true;
	}
	else return false;
}
template<typename ReturnType,typename ArgType>
class Task
{
	Task(std::function<ReturnType(ArgType)> f,ArgType a)
		:func(f)
		,arg(a)
	{};
private:
	std::function<ReturnType(ArgType)> func;
	ArgType arg;//参数；
};

template<typename ReturnType,typename ArgType>
class ThreadPool_SingleFunc
{
public:
	ThreadPool_SingleFunc(std::function<ReturnType(ArgType)> f,int min=2, int max=6);
	~ThreadPool_SingleFunc();
    void add_Task(ArgType&& task);
	int get_busyThread();
	int get_aliveThread();
public:
#if IS_VOID
#else
    ThreadSafeQueue<ReturnType>* result_queue;
    std::condition_variable result_cond;
    std::mutex result_mtx;
    std::unique_lock<std::mutex> result_lck;

#endif
private:
	/*int pthread_create(pthread_t *thread, const pthread_attr_t *attr,void* (*start_routine) (void*), void* arg)
	  第三个参数————入口函数：不可以是类的成员函数。
	*/
	static void* worker(void* arg);	//工作者线程
	static void* manager(void* arg);//管理者线程
private:
	const int max_tnum;
	const int min_tnum;
	int busy_tnum;
	int alive_tnum;
	bool is_shutdown;
	int exit_tnum;
	int busy_index;
	pthread_t* t_workers;	//工作线程组
	pthread_t t_manager;  //管理者线程；
	ThreadSafeQueue<ArgType>* data_queue;
	std::mutex* pool_mutex;
    std::condition_variable* pool_cond;
	std::function<ReturnType(ArgType)> func;
	
};





/**************************************************************************************************************/

template<typename ReturnType,typename ArgType>
ThreadPool_SingleFunc<ReturnType,ArgType>::ThreadPool_SingleFunc(std::function<ReturnType(ArgType)> f,int min, int max)
	:max_tnum(max)
	, min_tnum(min)
	, busy_tnum(0)
	, alive_tnum(min)
	, exit_tnum(0)
	, is_shutdown(false)
	,busy_index(0)
	,func(f)
    ,result_mtx()
    ,result_lck(result_mtx)
,result_cond()
{
#if IS_VOID
	result_queue = nullptr;
#else
    result_queue = new ThreadSafeQueue<ReturnType>;


    result_lck.unlock();
#endif // 年

		
	//初始化任务队列
	data_queue = new ThreadSafeQueue<ArgType>;
	if (isVaild(data_queue))return;
	//初始化互斥锁和条件变量
	pool_mutex = new std::mutex;
	if (isVaild(data_queue))return;
	pool_cond = new std::condition_variable;
	if (isVaild(data_queue))return;
	//初始化工作线程组
	t_workers = new pthread_t[max_tnum];
	if (isVaild(data_queue))return;
	memset(t_workers, 0, sizeof(pthread_t) * max_tnum);
	//创建工作线程
	for (int i = 0; i < alive_tnum; i++)
	{
		pthread_create(&t_workers[i], nullptr, worker, this);
	}
	//创建管理者线程
	pthread_create(&t_manager, nullptr, manager, this);
}

template<typename ReturnType,typename ArgType>
ThreadPool_SingleFunc<ReturnType,ArgType>::~ThreadPool_SingleFunc()
{
	this->is_shutdown = true;
	this->pool_mutex->lock();
	this->exit_tnum = this->alive_tnum;

	for (int i = 0; i < this->exit_tnum; i++)
	{
		this->pool_cond->notify_one();
	}
	this->pool_mutex->unlock();
	while (this->exit_tnum != 0)
	{
		usleep(5);
	}
	//任务队列
	delete this->data_queue;
	//结果队列

#if !IS_VOID
    delete this->result_queue;
#endif
	//工作线程组
	delete[] this->t_workers;
	//互斥器和条件变量
	delete this->pool_mutex;
	delete this->pool_cond;
}

template<typename ReturnType,typename ArgType>
void ThreadPool_SingleFunc<ReturnType,ArgType>::add_Task(ArgType&& task)
{
    this->data_queue->pushback(std::move(task));
	this->pool_cond->notify_one();
}

template<typename ReturnType,typename ArgType>
int ThreadPool_SingleFunc<ReturnType,ArgType>::get_busyThread()
{
	this->pool_mutex->lock();
	auto temp_busy = this->busy_tnum;
	this->pool_mutex->unlock();
	return  temp_busy;
}

template<typename ReturnType,typename ArgType>
int ThreadPool_SingleFunc<ReturnType,ArgType>::get_aliveThread()
{
	this->pool_mutex->lock();
	auto temp_alive = this->alive_tnum;
	this->pool_mutex->unlock();
	return  temp_alive;
}

template<typename ReturnType,typename ArgType>
void* ThreadPool_SingleFunc<ReturnType,ArgType>::worker(void* arg)
{
	ThreadPool_SingleFunc<ReturnType,ArgType>* pool = static_cast<ThreadPool_SingleFunc<ReturnType,ArgType>*>(arg);
	std::cout << "工作线程" << pthread_self() << "启动 " << std::endl;
	int num = 0;
	std::unique_lock<std::mutex> lck(*pool->pool_mutex);
	pool->busy_tnum++;
	lck.unlock();
	while (true)
	{
		std::cout << "工作线程" << pthread_self() << "已经完成任务：" <<num<< std::endl;
		lck.lock();
        //std::cout << "工作线程" << pthread_self() << "上了锁1" << std::endl;
		//检查任务队列是否为空
		if (pool->data_queue->get_size() == 0 && !pool->is_shutdown)
		{
            //std::cout << "工作线程" << pthread_self() << "上了锁2" << std::endl;
            pool->busy_tnum--;
            pool->busy_index++;
            //std::cout << "工作线程" << pthread_self() << "等待任务解开了锁2" << std::endl;
			pool->pool_cond->wait(lck);
            //std::cout << "工作线程" << pthread_self() << "上了锁3" << std::endl;
			pool->busy_tnum++;
			
		}
		//检查线程池是否关闭
		if (pool->is_shutdown || pool->exit_tnum > 0)
		{
			std::cout << "工作线程" << pthread_self() << "要关闭了" << std::endl;
			auto temp_array = pool->t_workers;
			lck.unlock();
            //std::cout << "工作线程" << pthread_self() << "解开了锁" << std::endl;
			auto pid = pthread_self();
			for (int i = 0; i < pool->max_tnum; i++)
			{
				if (temp_array[i] == pid)
				{
					lck.lock();
					pool->t_workers[i] = 0;
					pool->alive_tnum--;
					pool->busy_tnum--;
					pool->exit_tnum--;
					int temp_alive = pool->alive_tnum;
					lck.unlock();
					std::cout << "工作线程" << pid << "已退出 " << std::endl;
					std::cout << "当前剩余" << temp_alive << "个工作线程" << std::endl;
					break;
					;

				}
			}
			pthread_exit(NULL);
		}
		lck.unlock();
        //std::cout << "工作线程" << pthread_self() << "解开了锁13" << std::endl;
		ArgType temp_arg = pool->data_queue->pop();
#if IS_VOID
        pool->func(temp_arg);
#else
        pool->result_queue->pushback(pool->func(temp_arg));
        pool->result_cond.notify_one();
#endif
		num++;
	}
	return nullptr;
}

template<typename ReturnType,typename ArgType>
void* ThreadPool_SingleFunc<ReturnType,ArgType>::manager(void* arg)
{
	ThreadPool_SingleFunc<ReturnType,ArgType>* pool = static_cast<ThreadPool_SingleFunc<ReturnType,ArgType>*>(arg);
	//检测线程池是否关闭
	while (!pool->is_shutdown)
	{
		pool->pool_mutex->lock();
        //std::cout << "管理线程上了锁1" << std::endl;
		int temp_alive = pool->alive_tnum;
		int temp_busy = pool->busy_tnum;
		int index = pool->busy_index;
		long int task_num = pool->data_queue->get_size();
        //std::cout << "存活线程" <<temp_alive<< std::endl;
        //std::cout << "忙线程" <<temp_busy<< std::endl;
        //std::cout << "任务数量" <<task_num<< std::endl;
        //std::cout << "繁忙指数：" << index << std::endl;
        pool->pool_mutex->unlock();
        //std::cout << "管理线程解开了锁1" << std::endl;
		int range = 1;
		//检测是否需要减小线程数量
		if (pool->busy_index>5 && (temp_alive - range)>=pool->min_tnum)
		{
			pool->pool_mutex->lock();
            //std::cout << "管理线程上了锁4" << std::endl;
			pool->exit_tnum += range;
			pool->busy_index = 0;
			pool->pool_mutex->unlock();
            //std::cout << "管理线程解开了锁4" << std::endl;
			for (int i = 0; i < range; i++)
			{
				std::cout << "通知一个线程关闭" << std::endl;
				pool->pool_cond->notify_one();
			}
		}
		//检测是否需要增加线程数量
		else if (task_num > temp_alive && (temp_alive + range) <= pool->max_tnum)
		{
			pool->pool_mutex->lock();
            //std::cout << "管理线程上了锁2" << std::endl;
			auto temp_array = pool->t_workers;
			pool->pool_mutex->unlock();
            //std::cout << "管理线程解开了锁2" << std::endl;
			for (int i = 0; range > 0 && i < pool->max_tnum; i++)
			{

				if (temp_array[i] == 0)
				{

					pool->pool_mutex->lock();
                    //std::cout << "管理线程上了锁3" << std::endl;
					pool->alive_tnum++;

					pool->pool_mutex->unlock();
                    //std::cout << "管理线程解开了锁3" << std::endl;
					pthread_create(&pool->t_workers[i], nullptr, worker, pool);
					range--;

				}
			}

		}
		usleep(50);

	}
	std::cout << "管理者线程已退出" << std::endl;
	pthread_exit(NULL);
	return nullptr;
}

#endif
