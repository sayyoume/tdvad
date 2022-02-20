#pragma once
#include <queue>
#include <mutex>

typedef struct sDataStation
{
	std::string scode;
	unsigned int nStationFlag; //工位信息

}STDDATASTATION;

class  tdSafeQueueStation
{
public:
	tdSafeQueueStation();
	~tdSafeQueueStation();

	bool empty();
	void pushdata(STDDATASTATION* data);
	
	STDDATASTATION* popdata();

	void clearData();
	int size();

private:
	std::deque<STDDATASTATION*> m_queue;
	//std::deque<int> m_queue;
	std::mutex m_mutex;
};

