/*
 * Dispatcher.h
 *
 *  Created on: 2016年1月29日
 *      Author: jcz
 */

#pragma once

#include "EventHandler.h"
#include <list>
#include <map>

using namespace std;

namespace aizna
{

class Dispatcher
{
public:
	Dispatcher() : m_stopDispatch(false)
	{
		m_handlerMap.clear();
	}
	virtual ~Dispatcher()
	{
		Reset();
	}

	virtual void Reset()
	{
		HandlerMap::iterator mapIt = m_handlerMap.begin();
		for (; mapIt != m_handlerMap.end(); ++mapIt)
		{
			HandlerList& handlerList = mapIt->second;

			HandlerList::iterator listIt = handlerList.begin();
			for (; listIt != handlerList.end(); listIt++)
			{
				delete *listIt;
			}
		}
	}

public:
	template<typename T>
	void AddListener(int type, T* object, void (T::*func)())
	{
		if (false == HasListener(type, object, func))
		{
			EventHandler* handler = new EventHandler(object, func);
			m_handlerMap[type].push_back(handler);
		}
	}
	template<typename T, typename Event>
	void AddListener(int type, T* object, void (T::*func)(const Event&))
	{
		if (false == HasListener(type, object, func))
		{
			EventHandler1<Event>* handler = new EventHandler1<Event>(object,
					func);
			m_handlerMap[type].push_back(handler);
		}
	}

	template<typename T>
	void RemoveListener(int type, T* object, void (T::*func)())
	{
		HandlerMap::iterator mapIt = m_handlerMap.find(type);
		if (mapIt != m_handlerMap.end())
		{
			HandlerList& handlerList = mapIt->second;
			HandlerList::iterator listIt = handlerList.begin();
			for (; listIt != handlerList.end(); ++listIt)
			{
				IEventHandler* handler = (*listIt);
				if (handler->IsSame(object, func))
				{
					delete handler;
					handlerList.erase(listIt);
					if (0 == handlerList.size())
					{
						m_handlerMap.erase(mapIt);
					}
					return;
				}
			}
		}
	}

	template<typename T, typename Event>
	void RemoveListener(int type, T* object, void (T::*func)(const Event&))
	{
		HandlerMap::iterator mapIt = m_handlerMap.find(type);
		if (mapIt != m_handlerMap.end())
		{
			HandlerList& handlerList = mapIt->second;
			HandlerList::iterator listIt = handlerList.begin();
			for (; listIt != handlerList.end(); ++listIt)
			{
				if ((*listIt)->IsSame(object, func))
				{
					handlerList.erase(listIt);
					if (0 == handlerList.size())
					{
						m_handlerMap.erase(mapIt);
					}
					return;
				}
			}
		}
	}

	template<typename T>
	bool HasListener(int type, T* object, void (T::*func)())
	{
		HandlerMap::iterator mapIt = m_handlerMap.find(type);
		if (mapIt != m_handlerMap.end())
		{
			HandlerList& handlerList = mapIt->second;

			HandlerList::iterator listIt = handlerList.begin();
			for (; listIt != handlerList.end(); listIt++)
			{
				if ((*listIt)->IsSame(object, func))
				{
					return true;
				}
			}
		}
		return false;
	}

	template<typename T, typename Event>
	bool HasListener(int type, T* object, void (T::*func)(const Event&))
	{
		HandlerMap::iterator mapIt = m_handlerMap.find(type);
		if (mapIt != m_handlerMap.end())
		{
			HandlerList& handlerList = mapIt->second;

			HandlerList::iterator listIt = handlerList.begin();
			for (; listIt != handlerList.end(); listIt++)
			{
				if ((*listIt)->IsSame(object, func))
				{
					return true;
				}
			}
		}
		return false;
	}

	template<typename Event>
	void Dispatch(int type, const Event& event)
	{
		m_stopDispatch = false;
		HandlerList triggerList;

		HandlerMap::iterator mapIt = m_handlerMap.find(type);
		if (mapIt != m_handlerMap.end())
		{
			HandlerList& handlerList = mapIt->second;
			HandlerList::iterator listIt = handlerList.begin();
			for (; listIt != handlerList.end(); ++listIt)
			{
				IEventHandler* handler = *listIt;
				//防止回调方法中调用RemoveListener 进而引起宕机
				//需要先把将进行ontrriger的listener拷贝出来 再进行ontrriger
				triggerList.push_back(handler);
			}
		}

		HandlerList::iterator trigIt = triggerList.begin();
		for (; trigIt != triggerList.end(); ++trigIt)
		{
			if (m_stopDispatch)
				return;
			IEventHandler* handler = *trigIt;
			if (handler->GetArgsNum() == 1)
			{
				EventHandler1<Event>* eventHanedler =
						dynamic_cast<EventHandler1<Event>*>(handler);
				if (nullptr != eventHanedler)
				{
					eventHanedler->Invoke(event);
				}
			}
		}

		return;
	}

	void Dispatch(int type)
	{
		m_stopDispatch = false;
		HandlerList triggerList;

		HandlerMap::iterator mapIt = m_handlerMap.find(type);
		if (mapIt != m_handlerMap.end())
		{
			HandlerList& handlerList = mapIt->second;
			HandlerList::iterator listIt = handlerList.begin();
			for (; listIt != handlerList.end(); ++listIt)
			{
				IEventHandler* handler = *listIt;
				//防止回调方法中调用RemoveListener 进而引起宕机
				//需要先把将进行ontrriger的listener拷贝出来 再进行ontrriger
				triggerList.push_back(handler);
			}
		}

		HandlerList::iterator trigIt = triggerList.begin();
		for (; trigIt != triggerList.end(); ++trigIt)
		{
			if (m_stopDispatch)
				return;
			IEventHandler* handler = *trigIt;
			if (handler->GetArgsNum() == 0)
			{
				handler->Invoke();
			}
		}

		return;
	}

	// 在回调函数中使用此方法阻止事件派发
	void StopDispatch()
	{
		m_stopDispatch = true;
	}

private:
	typedef list<IEventHandler*> HandlerList;
	typedef map<int /*Event Type*/, HandlerList> HandlerMap;

private:
	HandlerMap m_handlerMap;
	bool m_stopDispatch;
};

}
