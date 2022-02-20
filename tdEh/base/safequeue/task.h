#pragma once
#include <queue>
#include <mutex>

class  tdTask
{
public:
	tdTask();
	~tdTask();

	bool empty();
	void pushTask(int data);
	
	int runTask();

	void clearData();
	int size();

private:
	//std::deque<STDDATA*> m_queue;
	std::deque<int> m_queue;
	//std::mutex m_mutex;
};

