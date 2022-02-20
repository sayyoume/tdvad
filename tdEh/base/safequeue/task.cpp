#include "task.h"


tdTask::tdTask()
{
}


tdTask::~tdTask()
{
}

bool tdTask::empty()
{
	//std::lock_guard<std::mutex> lock(m_mutex);
	return m_queue.empty();
}

void tdTask::pushTask(int data)
{
	//std::lock_guard<std::mutex> lock(m_mutex);
	m_queue.push_back(data);
}

int tdTask::runTask()
{
	//std::lock_guard<std::mutex> lock(m_mutex);
	if (m_queue.empty()) 
		return -100;

	int data = m_queue.front();
	m_queue.pop_front();

	return data;
}

void tdTask::clearData()	// 清空数据队列
{
	//std::lock_guard<std::mutex> lock(m_mutex);
	while (!m_queue.empty())
	{		
		m_queue.pop_front();
	}
}

int tdTask::size()
{
	return m_queue.size();
}
