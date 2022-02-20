#include "safequeue.h"


tdSafeQueue::tdSafeQueue()
{
}


tdSafeQueue::~tdSafeQueue()
{
}

bool tdSafeQueue::empty()
{
	std::lock_guard<std::mutex> lock(m_mutex);
	return m_queue.empty();
}

void tdSafeQueue::pushdata(STDDATA* data)
{
	std::lock_guard<std::mutex> lock(m_mutex);
	m_queue.push_back(data);
}

STDDATA* tdSafeQueue::popdata()
{
	std::lock_guard<std::mutex> lock(m_mutex);
	if (m_queue.empty()) 
		return nullptr;

	STDDATA* data = nullptr;
	data = m_queue.front();
	m_queue.pop_front();

	return data;
}

void tdSafeQueue::clearData()	// 清空数据队列
{
	std::lock_guard<std::mutex> lock(m_mutex);
	while (!m_queue.empty())
	{		
		STDDATA *pItem = m_queue.front();
		m_queue.pop_front();
		delete pItem;
	}
}

int tdSafeQueue::size()
{
	return m_queue.size();
}
