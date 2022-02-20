#include "safequeuestation.h"


tdSafeQueueStation::tdSafeQueueStation()
{
}


tdSafeQueueStation::~tdSafeQueueStation()
{
}

bool tdSafeQueueStation::empty()
{
	std::lock_guard<std::mutex> lock(m_mutex);
	return m_queue.empty();
}

void tdSafeQueueStation::pushdata(STDDATASTATION* data)
{
	std::lock_guard<std::mutex> lock(m_mutex);
	m_queue.push_back(data);
}

STDDATASTATION* tdSafeQueueStation::popdata()
{
	std::lock_guard<std::mutex> lock(m_mutex);
	if (m_queue.empty()) 
		return nullptr;

	STDDATASTATION* data = nullptr;
	data = m_queue.front();
	m_queue.pop_front();

	return data;
}

void tdSafeQueueStation::clearData()	// 清空数据队列
{
	std::lock_guard<std::mutex> lock(m_mutex);
	while (!m_queue.empty())
	{		
		STDDATASTATION *pItem = m_queue.front();
		m_queue.pop_front();
		delete pItem;
	}
}

int tdSafeQueueStation::size()
{
	return m_queue.size();
}
