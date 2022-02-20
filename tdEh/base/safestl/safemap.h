#ifndef __SAFEMAP__H__
#define __SAFEMAP__H_

#include <map>


template<typename Key, typename Val>
class safemap {
public:
	typedef typename std::map<Key, Val>::iterator this_iterator;
	typedef typename std::map<Key, Val>::const_iterator this_const_iterator;
	Val& operator [](const Key& key)
	{
		std::lock_guard<std::mutex> lk(mtx_);
		return dataMap_[key];
	}
	int erase(const Key& key)
	{
		std::lock_guard<std::mutex> lk(mtx_);
		return dataMap_.erase(key);
	}

	this_iterator find(const Key& key)
	{
		std::lock_guard<std::mutex> lk(mtx_);
		return dataMap_.find(key);
	}
	this_const_iterator find(const Key& key) const
	{
		std::lock_guard<std::mutex> lk(mtx_);
		return dataMap_.find(key);
	}
	
	this_iterator end()
	{
		return dataMap_.end();
	}

	this_const_iterator end() const
	{
		return dataMap_.end();
	}

private:
	std::map<Key, Val> dataMap_;
	std::mutex mtx_;
};


#endif // !__SAFEMAP__H__