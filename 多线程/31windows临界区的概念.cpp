#include <iostream>
#include <string>
#include<thread>
#include <vector>
#include<list>
#include<mutex>
#include <windows.h>
using namespace std;

//用成员函数作为线程初始化函数
#define __WINDOWS

//本类用于自动释放windows下的临界去，类似于lock_guard()
class CwinLock//这种类被称为RAII类（resource acquisition is initialation）
{
public:
	CwinLock(CRITICAL_SECTION*pCritem)//构造函数
	{
		m_pCritical = pCritem;
		EnterCriticalSection(m_pCritical);
	}
	~CwinLock()//析构函数
	{
		LeaveCriticalSection(m_pCritical);
	}
private:
	CRITICAL_SECTION *m_pCritical;
};


class A
{
public:
	//把收到的消息入到一个队列，子线程的启动函数
	void inMsgRecvQueue()
	{
		for (int i = 0; i < 10000; i++)
		{
			cout << "inMsgQueue插入一个元素" << i << endl;
#ifdef __WINDOWS
			//进入临界区
			//EnterCriticalSection(&my_winsec);
			//msgRecvQueue.push_back(i);//执行操作
			//LeaveCriticalSection(&my_winsec);//离开临界区

			CwinLock wlock(&my_winsec);//调用多次不会出错
			msgRecvQueue.push_back(i);//执行操作
			

#else
			std::lock_guard<mutex> my_lockguard(my_mutex);
			//my_mutex.lock();
			msgRecvQueue.push_back(i);//假设这个数字i就是收到的玩家的命令
			//my_mutex.unlock();
#endif
		}
	}
	
	//读共享数据函数的封装函数，使用lock_guard()
	bool outMsgprocess(int&command)
	{
#ifdef __WINDOWS
		EnterCriticalSection(&my_winsec);

		if (!msgRecvQueue.empty())
		{
			//消息队列不为空
			command = msgRecvQueue.front();//返回第一个元素
			msgRecvQueue.pop_front();//移除第一个元素
			LeaveCriticalSection(&my_winsec);
			return true;
		}
		LeaveCriticalSection(&my_winsec);
		return false;
#else

		my_mutex.lock();
		if (!msgRecvQueue.empty())
		{
			//消息队列不为空
			command = msgRecvQueue.front();//返回第一个元素
			msgRecvQueue.pop_front();//移除第一个元素
			my_mutex.unlock();
			return true;
		}
		my_mutex.unlock();
		return false;
		//所有分支都要有unlock()，两个出口必须有两个unlock()
#endif
	}
	//把数据从消息队列中取出的子线程
	void outMsgRecvQueue()
	{
		int command = 0;
		for (int i = 0; i < 10000; i++)
		{
			bool result = outMsgprocess(command);
			if (result == true)
			{
				cout << "取消息函数执行成功" << command << endl;
			}
			else
			{
				cout << "消息队列中的消息为空" << i << endl;
			}
		}

		cout << endl;
	}

	//定义一个widows临界区变量
#ifdef __WINDOWS
	A()
	{
		//初始化临界区,用之前必须初始化
		InitializeCriticalSection(&my_winsec);
	}
#endif
private:
	list<int>msgRecvQueue;//容器用来存放玩家发送过来的命令
	//创建一个互斥量的成员变量
	mutex my_mutex;
	//定义一个递归互斥量--没有使用
	recursive_mutex my_mutex2;

	//定义一个widows临界区变量
#ifdef __WINDOWS
	CRITICAL_SECTION my_winsec;
#endif

};

int main(void)
{
	A myobj;
	thread myOutMsgObj(&A::outMsgRecvQueue, &myobj);//第二个是引用才能保证线程中用的是统一个对象
	thread myInMsObj(&A::inMsgRecvQueue, &myobj);
	myOutMsgObj.join();
	myInMsObj.join();

	cout << "main线程" << endl;//最后执行这一句，整个线程退出
	system("pause");
	return 0;
}


/*
 * windows下的临界区非常类似于c++11中的mutex;临界区使用前必须初始化
 * 需要包含#include<windows.h>
 * (1)进入临界区（和加锁一样）(2)执行操作(3)离开临界区（和解锁一样unlock()）
 * 
 * 多次进入临界区实验
 *在同一个线程中，多次进入临界区（相同的临界区变量），就要多次离开临界区，要配对，使用windows临界区是可以的
 *进入几次临界区就要离开几次临界区。
 *c++11中互斥量mutex是不可以加多次锁，解多次锁。--相同的mutex不允许在同一个线程中调用多次。
 *
 *
 *自动析构技术
 *std::lock_guard()技术就是使用自动析构技术。
 *
 *
 *递归的独占互斥量 recursive mutex--可以同一个线程多次lock(),用法与mutex相同
 *如果使用了两次lock()--->应该考虑代码是否有优化的空间
 *
 *带超时的互斥量 std::timed_mutex--超时功能的互斥量,等待超过时间不阻塞
 *	多了接口  try_lock_for()   try_lock_until()--等待未来的一个世界节点
 std::chrono::milliseconds timeout(100);
 *if(my_mutex2.try_lock_for(timeout))
 *或者 if (my_mutex2.try_lock_until(chrono::stead_clock::now()+timeout))
 *{
 *	没有拿到锁头
 *}
 *else
 *{
 *	进入睡眠100ms，看下次能不能拿到锁
 *}
 *
 *std::recursive_timed_mutex带超时功能的递归互斥量
 */