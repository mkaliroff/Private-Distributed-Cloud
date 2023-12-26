#ifndef WAITABLE_QUEUE_HPP
#define WAITABLE_QUEUE_HPP

#include <queue>                // std::queue
#include <condition_variable>   // std::condition_variable
#include <mutex>                // std::mutex
#include <chrono>               // std::nanoseconds

namespace pdc
{
template <typename T, class CONTAINER = std::queue<T>>
class WaitableQueue
{
public:
	WaitableQueue() = default;
	~WaitableQueue() = default;
	WaitableQueue(const WaitableQueue &rhs) = delete;
	WaitableQueue &operator=(const WaitableQueue &rhs) = delete;

	void Push(const T &data);
	void Pop(T &outparam);
	bool Pop(T &outparam, std::chrono::nanoseconds timeout);
	bool IsEmpty(void) const;

private:
	CONTAINER m_container_queue;
	mutable std::mutex m_mutex;
	std::condition_variable m_cond_var;
};

/******************************************************************************/
template <typename T, class CONTAINER>
void WaitableQueue<T,CONTAINER>::Push(const T &data)
{
	std::lock_guard<std::mutex> lock(m_mutex);
	//std::cout << "push" << data << std::endl;
	m_container_queue.push(data);
	m_mutex.unlock();
	m_cond_var.notify_one();
}

/******************************************************************************/
template <typename T, class CONTAINER>
void WaitableQueue<T,CONTAINER>::Pop(T &outparam)
{
	std::unique_lock<std::mutex> lock(m_mutex);
	m_cond_var.wait(lock, [this]{return !m_container_queue.empty();});
	outparam = m_container_queue.front();
	//std::cout << "pop" << outparam << std::endl;
	m_container_queue.pop();
}

/******************************************************************************/
template <typename T, class CONTAINER>
bool WaitableQueue<T,CONTAINER>::Pop(T &outparam, std::chrono::nanoseconds timeout)
{
	std::unique_lock<std::mutex> lock(m_mutex);
	bool status = m_cond_var.wait_for(lock, timeout, [this]{return !m_container_queue.empty();});

	if(status)
	{
		outparam = m_container_queue.front();
		m_container_queue.pop();
	}
	
	return status;
}

/******************************************************************************/
template <typename T, class CONTAINER>
bool WaitableQueue<T,CONTAINER>::IsEmpty(void) const
{
	std::unique_lock<std::mutex> lock(m_mutex);
	return m_container_queue.empty();
}


/******************************************************************************/

}
#endif // WAITABLE_QUEUE_HPP
