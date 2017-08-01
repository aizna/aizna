/*
 * Main.cpp
 *
 *  Created on: 2016年12月17日
 *      Author: Administrator
 */

#include <event/Dispatcher.h>
#include <iostream>

using namespace std;
using namespace aizna;

class Aevent
{
public:
	Aevent(){};
};

class Player
{
public:
	Player()
	{
		ID = 0;
	}
	~Player()
	{
		ID = 0;
	}

	void Say()
	{
		cout << "I am Num." << ID << " Player." << endl;
	}

//	void Say(Event*)
//	{
//		cout << "I am Num." << ID << " Player. Recv Event! " << endl;
//	}

	void Run(const Aevent& event)
	{
		cout << "GO GO GO." << endl;
	}

	int ID;
};


static const int EVENT_SAY = 1;
static const int EVENT_RUN = 2;

int main()
{

	Dispatcher dispacher;


	Player p1;
	p1.ID = 1;

	// 添加事件侦听
	dispacher.AddListener(EVENT_SAY, &p1, &Player::Say);

	cout<<endl <<"-----------------------------测试是否区分有参和无参"<<endl;

	// 派发事件
	dispacher.Dispatch(EVENT_SAY, Aevent());
	dispacher.Dispatch(EVENT_SAY);

	cout<<endl <<"-----------------------------测试重复添加事件侦听，是否去重"<<endl;

	// 测试重复添加事件侦听，是否去重
	dispacher.AddListener(EVENT_RUN, &p1, &Player::Run);
	dispacher.AddListener(EVENT_RUN, &p1, &Player::Run);

	dispacher.Dispatch(EVENT_RUN);
	dispacher.Dispatch(EVENT_RUN, Aevent());

	cout<<endl <<"-----------------------------测试是否能区分不同对象"<<endl;

	Player p2;
	p2.ID = 2;

	// 测试是否能区分不同对象
	dispacher.AddListener(EVENT_SAY, &p2, &Player::Say);

	dispacher.Dispatch(EVENT_SAY);
	dispacher.Dispatch(EVENT_SAY);

	cout<<endl << "-----------------------------测试事件移除"<<endl;

	// 测试事件移除
	dispacher.RemoveListener(EVENT_SAY, &p2, &Player::Say);
	dispacher.Dispatch(EVENT_SAY);

	// 测试移除已被移除的事件，会不会出问题
	dispacher.RemoveListener(EVENT_SAY, &p2, &Player::Say);
	dispacher.RemoveListener(EVENT_SAY, &p1, &Player::Say);

	cout<<endl << "-----------------------------测试对象清除后会出现的问题"<<endl;
	//测试对象清除后会出现的问题
	Player* p3 = new Player;
	p3->ID = 3;
	dispacher.AddListener(EVENT_SAY, p3, &Player::Say);
	delete p3;
	dispacher.Dispatch(EVENT_SAY);

	// 事件只会被注册一次，移除时不小心多次移除也不会有问题。
	// 事件添加后要记得移除，否则一直在派发器中
	// TODO 对象添加事件后，没移除事件就被销毁，想办法从事件派发器中移出

	cout<<endl;

	return 0;
}
