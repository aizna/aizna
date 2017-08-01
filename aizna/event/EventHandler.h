/*
 * EventHandler.h
 *
 *  Created on: 2016年1月29日
 *      Author: jcz
 */

#pragma once
#include <string.h>
#include <functional>

namespace aizna
{

class IEventHandler
{
public:
	IEventHandler():m_target(nullptr),m_func(nullptr){}
	virtual ~IEventHandler(){}

	int GetArgsNum() const
	{
		return m_argsNum;
	}

	template<typename T>
	bool IsSame(T* object, void(T::*func)())
	{
		void *data[2];
		memcpy(data, &func, sizeof(func));
		return m_target == object && m_func == data[0];
	}

	template<typename T, typename Event>
	bool IsSame(T* object, void(T::*func)(const Event&))
	{
		void *data[2];
		memcpy(data, &func, sizeof(func));
		return m_target == object && m_func == data[0];
	}

	virtual void Invoke(){};

protected:
	void*      	m_target;
	void*       m_func;
	int			m_argsNum;
};

//=====================================================================

// 没有参数的Handler
class EventHandler: public IEventHandler
{
public:
	virtual ~EventHandler(){};

	template<typename T>
	EventHandler(T* object, void(T::*func)())
	{
		m_argsNum = 0;
		m_target = object;
		void *data[2];
		memcpy(data, &func, sizeof(func));
		m_func = data[0];

		m_handler = std::bind(func, object);
	}

	// EventCallback
public:
	virtual void Invoke()
	{
		(m_handler)();
	}

private:
	std::function<void()>    m_handler;
};

//=====================================================================

// 带一个参数的Handler
template<typename Event>
class EventHandler1: public IEventHandler
{
public:
	virtual ~EventHandler1(){};

	template<typename T>
	EventHandler1(T* object, void (T::*func)(const Event&))
	{
		m_argsNum = 1;
		m_target = object;
		void *data[2];
		memcpy(data, &func, sizeof(func));
		m_func = data[0];

		m_handler = std::bind(func, object, std::placeholders::_1);
	}

	// EventCallback
public:
	virtual void Invoke(const Event& event)
	{
		(m_handler)(event);
	}

private:
    std::function<void(const Event&)>    m_handler;
};

}
