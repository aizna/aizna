/*
 * Pool.h 简单对象池的实现，被管理的对象需要继承PoolObject
 *
 *  Created on: 2017年3月2日
 *      Author: jcz
 */
#pragma once

#include "Singleton.h"
#include <map>
#include <queue>

namespace aizna
{

class PoolObject
{
public:
	PoolObject():_index(0)	{}
    virtual ~PoolObject()	{}

public:
    virtual void Reset() = 0;
    int Index() const { return _index; }
private:
    int _index;
};

// T为对象类型，不是对象指针，Pool会把转为指针存入map
template <typename T>
class Pool
{
public:
	Pool();
    virtual ~Pool();

public:
    T* 		Create();
    T*		Find(int index);
    void 	Release(int index);
	void 	Release(T* object);

    // Getter and Setter
public:
	int 	GetFreeCount() const { return m_freeCount; }
	int 	GetUsedCount() const { return m_usedCount; }
	int 	GetAllCount()  const { return m_maxIndex;  }

private:
	int		m_maxIndex;
	int 	m_freeCount;
	int		m_usedCount;

    typedef std::queue<T*>		ObjectQueue;
    typedef std::map<int,T*>	ObjectMap;

    ObjectQueue m_queue;
    ObjectMap m_objectMap;
};

#define DECLARE_POOL(object) \
class object##Manager : public Singleton<object##Manager>, public Pool<object> {};

//-----------------------------------------------------------------------------------------------

template<typename T>
Pool<T>::Pool() :
	m_maxIndex(0),m_freeCount(0), m_usedCount(0)
{
}

template< typename T>
Pool<T>::~Pool()
{
	while (!m_queue.empty())
	{
		T* object = m_queue.front();
		m_queue.pop();
		delete object;
	}

	typename ObjectMap::iterator it = m_objectMap.begin();
	for (; it != m_objectMap.end(); ++it)
	{
		delete it->second;
	}
	m_objectMap.clear();
}

template<typename T>
inline T* Pool<T>::Create()
{
	if (!m_queue.empty())
	{
		T* object = m_queue.front();
		if(nullptr == object) return nullptr;
		PoolObject* poolObject = dynamic_cast<PoolObject*>(object);
		int& index = *((int*)poolObject+2);
		m_objectMap.insert(typename ObjectMap::value_type(index, object));
		m_queue.pop();
		m_freeCount--;
		m_usedCount++;
		return object;
	}

	T* newObject = new T();
	PoolObject* newPoolObject = dynamic_cast<PoolObject*>(newObject);
	if(nullptr == newPoolObject)
	{
		delete newObject;
		return nullptr;
	}
	m_maxIndex++;
	m_usedCount++;
	*((int*)newObject+2) = m_maxIndex;
	m_objectMap.insert(typename ObjectMap::value_type(m_maxIndex, newObject));
	return newObject;
}

template<typename T>
inline T* Pool<T>::Find(int index)
{
	typename ObjectMap::iterator it = m_objectMap.find(index);
	if (it != m_objectMap.end())
	{
		T* object = it->second;
		if(nullptr == object)
		{
			m_objectMap.erase(it);
			return nullptr;
		}
		return object;
	}
	return nullptr;
}

template<typename T>
inline void Pool<T>::Release(int index)
{
	typename ObjectMap::iterator it = m_objectMap.find(index);
	if (it != m_objectMap.end())
	{
		T* object = it->second;
		if (nullptr != object)
		{
			PoolObject* poolObject = dynamic_cast<PoolObject*>(object);
			if(nullptr != poolObject)
			{
				/**
				 *	object->~T(); 如果直接调用析构函数来代替 poolObject->Reset();
				 *	会出现double fee的错误，所以在Object类里写了一个纯虚函数Reset()
				 */
				poolObject->Reset();
				m_queue.push(object);
				m_freeCount++;
			}
		}

		m_objectMap.erase(it);
		m_usedCount--;
	}
}

template<typename T>
inline void Pool<T>::Release(T* object)
{
	if(nullptr == object) return;
	PoolObject* poolObject = dynamic_cast<PoolObject*>(object);
	if(nullptr == poolObject) return;

	int& index = *((int*)poolObject+2);
	Release(index);
}

}
