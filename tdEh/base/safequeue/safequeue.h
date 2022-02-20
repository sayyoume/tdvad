#pragma once
#include <queue>
#include <mutex>



typedef struct sData
{
	long nTime;
	int  nTagId;
}STDDATA;

class  tdSafeQueue
{
public:
	tdSafeQueue();
	~tdSafeQueue();

	bool empty();
	void pushdata(STDDATA* data);
	
	STDDATA* popdata();

	void clearData();
	int size();

private:
	std::deque<STDDATA*> m_queue;
	//std::deque<int> m_queue;
	std::mutex m_mutex;
};

