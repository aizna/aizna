/*
 * Manager.h 简单管理器的实现，被管理的对象需要继承Object
 *
 *  Created on: 2017年3月2日
 *      Author: jcz
 */
#pragma once

#include "Singleton.h"
#include <map>

namespace aizna
{

class Object
{
public:
	Object():_index(0)	{}
    virtual ~Object()	{}

public:
    int Index() const { return _index; }
private:
    int _index;
};

// T为对象类型，不是对象指针，Manager会把转为指针存入map
template <typename T>
class Manager
{
public:
	Manager();
    virtual ~Manager();

public:
    T* 		Create();
    T*		Find(int index);
    void 	Release(int index);
	void 	Release(T* object);

    // Getter and Setter
public:
	int 	GetUsedCount() const { return m_usedCount; }

private:
	int		m_maxIndex;
	int		m_usedCount;

    typedef std::map<int,T*>	ObjectMap;

    ObjectMap m_objectMap;
};


#define DECLARE_MANAGER(object) \
class object##Manager : public Singleton<object##Manager>, public Manager<object> {};

//-----------------------------------------------------------------------------------------------

template<typename T>
Manager<T>::Manager() :
	m_maxIndex(0), m_usedCount(0)
{
}

template< typename T>
Manager<T>::~Manager()
{
	typename ObjectMap::iterator it = m_objectMap.begin();
	for (; it != m_objectMap.end(); ++it)
	{
		delete it->second;
	}
	m_objectMap.clear();
}

template<typename T>
inline T* Manager<T>::Create()
{
	T* newObject = new T();
	m_maxIndex++;
	m_usedCount++;
	if(m_maxIndex < 0) m_maxIndex =1;
	*((int*)newObject+2) = m_maxIndex;
	m_objectMap.insert(typename ObjectMap::value_type(m_maxIndex, newObject));
	return newObject;
}

template<typename T>
inline T* Manager<T>::Find(int index)
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
inline void Manager<T>::Release(int index)
{
	typename ObjectMap::iterator it = m_objectMap.find(index);
	if (it != m_objectMap.end())
	{
		T* object = it->second;
		if (nullptr != object)
		{
			delete object;
		}

		m_objectMap.erase(it);
		m_usedCount--;
	}
}

template<typename T>
inline void Manager<T>::Release(T* object)
{
	if(nullptr == object) return;
	Object* poolObject = dynamic_cast<Object*>(object);
	if(nullptr == poolObject) return;

	int& index = *((int*)poolObject+2);
	Release(index);
}

}
