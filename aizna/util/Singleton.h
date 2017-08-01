/*
 * Singleton.h 仅支持C++11标准，如果是之前的标准，需要考虑线程安全,C++11静态变量已处理多线程问题
 *
 *  Created on: 2016年11月2日
 *      Author: jcz
 */
#pragma once

template <typename T>
class Singleton
{
protected:
	Singleton() {}
	virtual  ~Singleton() {}
private:
	Singleton(const Singleton&) = delete;
	Singleton& operator=(const Singleton&) = delete;
public:
	static T& Instance()
	{
		static T instance;
		return instance;
	}
};

