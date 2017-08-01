/*
 * Main.cpp
 *
 *  Created on: 2016年11月14日
 *      Author: jcz
 */
#include <util/Singleton.h>
#include <util/Factory.h>
#include <iostream>

using namespace std;
using namespace aizna;

class Base
{
public:
	virtual void Say()
	{
		cout << "I am Base" << endl;
	}
};

class A: public Base
{
public:
	virtual void Say()
	{
		cout << "I am A" << endl;
	}
};

class B: public Base
{
public:
	virtual void Say()
	{
		cout << "I am B" << endl;
	}
};

class F: public Factory<Base>, public Singleton<F>
{
public:
	F()
	{
		Register<A>(1);
		Register<B>(2);
	}
};

int main()
{
	Base* base1 = F::Instance().Create(1);
	base1->Say();

	Base* base2 = F::Instance().Create(2);
	base2->Say();
}
