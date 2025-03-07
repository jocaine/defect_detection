#ifndef _THREADSAFEQUEUE_HPP
#define _THREADSAFEQUEUE_HPP
#include<mutex>
#include<queue>
#include<iostream>
const size_t MAX_SIZE=50;
const size_t MIN_SIZE =10;
template <typename T>
class ThreadSafeQueue
{
public:
	ThreadSafeQueue(bool s=0);
    bool pushback(T&& element);
	T pop();
	int get_size();
	void set_expand();
	void set_dicard();
private:
	std::queue<T> TS_queue;
	std::mutex mutex;
	size_t capacity;
	bool state;

};

/**************************************************************************************************************/
template<typename T>
inline ThreadSafeQueue<T>::ThreadSafeQueue(bool s)
	:capacity(MIN_SIZE)
	, state(s)
{
}

template<typename T>
bool ThreadSafeQueue<T>::pushback(T&& element)
{
	if (get_size() == capacity)
	{
		if (state)
		{
			//队列容量达到最大，采取丢弃策略
			return false;
		}
		else
		{
			//扩容策略
			capacity = 1.5 * capacity;
		}
	}
    std::cout<<"有数据进入"<<std::endl;
	this->mutex.lock();
	TS_queue.push(element);
	this->mutex.unlock();
	return true;
}

template<typename T>
T ThreadSafeQueue<T>::pop()
{
	if (get_size()<=0)
	{
		std::cout << "empty"  << std::endl;
		if (capacity != MIN_SIZE)
		{
			mutex.lock();
			capacity = MIN_SIZE;
			mutex.unlock();
		}
		return T();
	}
	
	this->mutex.lock();
	T temp = std::move(TS_queue.front());
	TS_queue.pop();
	this->mutex.unlock();
	return temp;
}

template<typename T>
int ThreadSafeQueue<T>::get_size()
{
	mutex.lock();
	auto size = TS_queue.size();
	mutex.unlock();
	return size;
}

template<typename T>
void ThreadSafeQueue<T>::set_expand()
{
	this->state = 0;
}

template<typename T>
void ThreadSafeQueue<T>::set_dicard()
{
	this->state = 1;
}

#endif // !_THREADSAFEQUEUE_H
