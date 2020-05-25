#include <iostream>
#include <string>
#include<thread>
#include <vector>

using namespace std;
//线程入口函数
void MyPrint(int num)
{
	cout << "这是子线程的入口函数，子线程开始执行" << num<<endl;

	cout << "线程结束执行了" << num<<endl;
	return;
}

int main(void)
{
	vector<thread> mythread;
	//创建10个线程,入口统一使用MyPrint
	for (int i = 0; i<10; i++)
	{
		//创建线程，并执行
		mythread.push_back(thread(MyPrint, i));
	}
	//等待10个子线程返回
	for (auto iter = mythread.begin(); iter != mythread.end();iter++)
	{
		iter->join();
	}


	cout << "main线程" << endl;//最后执行这一句，整个线程退出
	system("pause");
	return 0;
}
/*
 *（1） 多个线程的执行顺序是无规律的，和操作系统的内部调用机制有关。
 * （2）主线程等待了所有的子线程执行完毕，主线程才退出。推荐这种join的写法，容易写出稳定的程序
 * （3）创建了多个线程并且放在一个动态数组容器中。方便创建和管理多个线程
 */

