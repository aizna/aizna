/*
 * Main.cpp
 *
 *  Created on: 2016年11月14日
 *      Author: jcz
 */

#include <iostream>
#include <util/Manager.h>

using namespace std;
using namespace aizna;

class Player : public Object
{
public:
	Player(){}
	~Player(){}
};

DECLARE_MANAGER(Player)

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
