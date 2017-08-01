/*
 * Main.cpp
 *
 *  Created on: 2016年11月14日
 *      Author: jcz
 */

#include <iostream>
#include <util/Pool.h>

using namespace std;
using namespace aizna;

class Player : public PoolObject
{
public:
	Player(){}
	~Player(){}

	virtual void Reset()
	{
		m_uid = 0;
	}

private:
	int m_uid;
};

DECLARE_POOL(Player)

int main()
{
	Player* p1 = PlayerManager::Instance().Create();
	cout << p1->Index() << endl;

	Player* p2 = PlayerManager::Instance().Create();
	cout << p2->Index() << endl;

	cout << PlayerManager::Instance().GetUsedCount() << endl;

	PlayerManager::Instance().Release(p1);

	cout << PlayerManager::Instance().GetUsedCount() << endl;

	PlayerManager::Instance().Release(p1);

	cout << PlayerManager::Instance().GetUsedCount() << endl;

	return 0;
}
