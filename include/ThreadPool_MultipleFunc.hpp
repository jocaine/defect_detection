#ifndef _MULTIPLEFUNCTHREADPOOL_HPP
#define _MULTIPLEFUNCTHREADPOOL_HPP
#include"ThreadSafeQueue.hpp"
#include<pthread.h>
#include<mutex>
#include<condition_variable>
#include<iostream>
#include<string>
#include<string.h>
#include<unistd.h>
#include<functional>
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
class ThreadPool
{
public:
	ThreadPool(int min=2, int max=6);
	~ThreadPool();
	void add_Task(Task<ReturnType,ArgType>& task);
	int get_busyThread();
	int get_aliveThread();
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
	pthread_t* t_workers;	//工作线程组
	pthread_t t_manager;  //管理者线程；
	ThreadSafeQueue<Task<ReturnType,ArgType>>* task_queue;
	std::mutex* pool_mutex;
	std::condition_variable* pool_cond;
};





/**************************************************************************************************************/

template<typename ReturnType,typename ArgType>
ThreadPool<ReturnType,ArgType>::ThreadPool(int min, int max)
	:max_tnum(max)
	, min_tnum(min)
	, busy_tnum(0)
	, alive_tnum(min)
	, exit_tnum(0)
	, is_shutdown(false)
{
	//初始化任务队列
	task_queue = new ThreadSafeQueue<Task<ReturnType,ArgType>>;
	if (isVaild(task_queue))return;
	//初始化互斥锁和条件变量
	pool_mutex = new std::mutex;
	if (isVaild(task_queue))return;
	pool_cond = new std::condition_variable;
	if (isVaild(task_queue))return;
	//初始化工作线程组
	t_workers = new pthread_t[max_tnum];
	if (isVaild(task_queue))return;
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
ThreadPool<ReturnType,ArgType>::~ThreadPool()
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
	delete this->task_queue;
	//工作线程组
	delete[] this->t_workers;
	//互斥器和条件变量
	delete this->pool_mutex;
	delete this->pool_cond;
}

template<typename ReturnType,typename ArgType>
void ThreadPool<ReturnType,ArgType>::add_Task(Task<ReturnType,ArgType>& task)
{
	this->pool_mutex->lock();
	this->task_queue->pushback(task);
	this->pool_cond->notify_one();
	this->pool_mutex->unlock();
}

template<typename ReturnType,typename ArgType>
int ThreadPool<ReturnType,ArgType>::get_busyThread()
{
	this->pool_mutex->lock();
	auto temp_busy = this->busy_tnum;
	this->pool_mutex->unlock();
	return  temp_busy;
}

template<typename ReturnType,typename ArgType>
int ThreadPool<ReturnType,ArgType>::get_aliveThread()
{
	this->pool_mutex->lock();
	auto temp_alive = this->alive_tnum;
	this->pool_mutex->unlock();
	return  temp_alive;
}

template<typename ReturnType,typename ArgType>
void* ThreadPool<ReturnType,ArgType>::worker(void* arg)
{
	ThreadPool<ReturnType,ArgType>* pool = static_cast<ThreadPool<ReturnType,ArgType>*>(arg);
	while (true)
	{
		pool->pool_mutex->lock();
		if (pool->task_queue->get_size() == 0 && !pool->is_shutdown)
		{
			pool->busy_tnum--;
			std::unique_lock<std::mutex> lck(*pool->pool_mutex);
			pool->pool_cond->wait(lck);

		}
		if (pool->is_shutdown || pool->exit_tnum > 0)
		{
			auto temp_array = pool->t_workers;
			pool->pool_mutex->unlock();
			auto pid = pthread_self();
			for (int i = 0; i < pool->max_tnum; i++)
			{
				if (temp_array[i] == pid)
				{
					pool->pool_mutex->lock();
					pool->t_workers[i] = 0;
					pool->alive_tnum--;
					pool->exit_tnum--;
					int temp_alive = pool->alive_tnum;
					pool->pool_mutex->unlock();
					std::cout << "工作线程" << pid << "已退出 " << std::endl;
					std::cout << "当前剩余" << temp_alive << "个工作线程" << std::endl;
					break;

				}
			}
			pthread_exit(NULL);
		}
		Task<ReturnType,ArgType> temp_task = pool->task_queue->pop();
		pool->busy_tnum++;
		pool->pool_mutex->unlock();
		temp_task.function(temp_task.arg);
		delete temp_task.arg;


	}
	return nullptr;
}

template<typename ReturnType,typename ArgType>
void* ThreadPool<ReturnType,ArgType>::manager(void* arg)
{
	ThreadPool<ReturnType,ArgType>* pool = static_cast<ThreadPool<ReturnType,ArgType>*>(arg);
	//检测线程池是否关闭
	while (!pool->is_shutdown)
	{
		pool->pool_mutex->lock();
		int temp_alive = pool->alive_tnum;
		int temp_busy = pool->busy_tnum;
		int task_num = pool->task_queue->get_size();
		pool->pool_mutex->unlock();
		int range = 1;
		//检测是否需要增加线程数量
		if (task_num > temp_alive && (temp_alive + range) < pool->max_tnum)
		{
			pool->pool_mutex->lock();
			auto temp_array = pool->t_workers;
			pool->pool_mutex->unlock();
			for (int i = 0; range > 0 && i < pool->max_tnum; i++)
			{

				if (temp_array[i] == 0)
				{
					pool->pool_mutex->lock();
					pthread_create(&pool->t_workers[i], nullptr, worker, pool);
					pool->alive_tnum++;
					pool->pool_mutex->lock();
					range--;

				}
			}

		}
		//检测是否需要减小线程数量
		else if (temp_busy<temp_alive && (temp_alive - range)>pool->min_tnum)
		{
			pool->pool_mutex->lock();
			pool->exit_tnum += range;
			pool->pool_mutex->unlock();
			for (int i = 0; i < range; i++)
			{
				pool->pool_cond->notify_one();
			}
		}
		usleep(5);

	}
	std::cout << "管理者线程已退出" << std::endl;
	pthread_exit(NULL);
	return nullptr;
}

#endif
