#include <iostream>
#include <string>
#include<thread>
#include <future>

using namespace std;
//int g_mycout = 0;
//std::mutex g_mutex;//引入互斥量来解决加法问题
//void mythread()
//{
//	for (int i = 0; i < 1000000;i++)
//	{
//		g_mutex.lock();
//		g_mycout++;
//		g_mutex.unlock();
//	}
//}

//
//std::atomic<int> g_mycout = 0;//封装了一个类型为原子类型的int，我可以像操作普通变量一样
//void mythread()
//{
//	for (int i = 0; i < 1000000; i++)
//	{
//		g_mycout++;//已经是一个atomic对象了，操作不会被打断
//		
//	}
//}

//线程退出标记,这里是元组操作
std::atomic<bool> isEnd = false;
void mythread()
{
	chrono::milliseconds dura(1000);
	std::this_thread::sleep_for(dura);
	while (isEnd==false)
	{
		//线程没有退出。所以本线程可以做自己想做的事情
		cout << "线程运行中" << endl;

	}
}
int main(void)
{
	thread	t1(mythread);
	thread	t2(mythread);
	

	chrono::milliseconds dura(4000);
	std::this_thread::sleep_for(dura);
	//s使用原子操作，让线程自动结束
	isEnd = true;

	t1.join();
	t2.join();
	cout << "g_mycout=" << isEnd<<endl;
	system("pause");
	return 0;
}

/*
 * 原子操作std::automic
 * 原子操作概念引出
 * 互斥量：多个线程编程中，保护共享数据，上锁，解锁
 * 有两个线程，一个变量，一个线程向变量写值，一个线程只读变量的值
 * 一个读线程，一个写线程，解决方案----
 * 向同一个全局变量写100000次
 * （1）互斥量（2）原子操作
 * 
 * //大家可以把原子操作理解成无锁技术的并发。也可以理解原子操作是不会被打断的代码段
 * 原子操作比互斥量效率要高
 * 互斥量一般可以针对多行代码，原子操作一般都是针对一个变量，而不i是一个代码段
 * 原子操作一般指不可分割的操作，这个操作要么完成，要么没做，不存在中间状态。
 * c++中使用atomic类模板来实现
 * 
 *原子操作--- 基本的用法范例
 *
 *心得：原子操作只针对一个变量，一般用原子操作进行计数，如发送的数据包，
 *接收的数据包
 *
 */