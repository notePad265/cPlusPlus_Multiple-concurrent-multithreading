#include <iostream>
#include <string>
#include<thread>
#include <future>

using namespace std;


std::atomic<int> g_mycout = 0;//封装了一个类型为原子类型的int，我可以像操作普通变量一样
void mythread()
{
	for (int i = 0; i < 1000000; i++)
	{
		//g_mycout++;//已经是一个atomic对象了，操作不会被打断
		g_mycout += 1;//可以
		//g_mycout = g_mycout + 1;//这种操作不可以，不是针对所有操作符都是可以的

	}
}


int main(void)
{
	thread	t1(mythread);
	thread	t2(mythread);

	t1.join();
	t2.join();
	cout << "g_mycout=" << g_mycout << endl;
	system("pause");
	return 0;
}

/*
*g_mycout = g_mycout + 1;//这种操作不可以，不是针对所有操作符都是可以的
*
*/