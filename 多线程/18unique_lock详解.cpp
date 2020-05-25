#include <iostream>
#include <string>
#include<thread>
#include <vector>
#include<list>
#include<mutex>
using namespace std;

//用成员函数作为线程初始化函数

class A
{
public:
	//通过函数返回一个unique_lock对象
	unique_lock<mutex> rtn_unique_lock()
	{
		unique_lock<mutex> temGuard(my_mutex1);
		return temGuard;//从函数返回一个局部的unique_lock是可以的
		//移动构造函数，系统临时生成一个临时对象。
	}


	//把收到的消息入到一个队列，子线程的启动函数
	void inMsgRecvQueue()
	{
		for (int i = 0; i < 10000; i++)
		{
			cout << "inMsgQueue插入一个元素" << i << endl;
		
			//lock_guard<mutex> myGuard2(my_mutex1);
			//unique_lock<mutex> myGuard1(my_mutex1,try_to_lock);
			//如果拿到了锁
			//if (myGuard1.owns_lock())
			//{
			//	msgRecvQueue.push_back(i);//假设这个数字i就是收到的玩家的命令

			//}
			//else
			//{
			//	cout << "没有拿到锁----------------------" << endl;
			//}


			//unique_lock<mutex> myGuard1(my_mutex1,defer_lock);
			//没有加锁的my_mutex1
			//加锁之后不用自己解锁
			///*myGuard1.lock();*/
			//if (myGuard1.try_lock()==true)
			//{
			//	msgRecvQueue.push_back(i);
			//}
			//else
			//{
			//	cout << "没有拿到锁----------------------" << endl;
			//}


		/*	unique_lock<mutex> myGuard1(my_mutex1);
			mutex*ptx = myGuard1.release();
			msgRecvQueue.push_back(i);
			ptx->unlock();
*/


			//所有权转移
			unique_lock<mutex> myGuard1(my_mutex1);
			unique_lock<mutex> myGuard2(move(myGuard1));

			mutex*ptx = myGuard2.release();
			msgRecvQueue.push_back(i);
			ptx->unlock();



		}
	}
	//读共享数据函数的封装函数
	bool outMsgprocess(int&command)
	{
		
		//lock_guard<mutex> myGuard1(my_mutex1);
		unique_lock<mutex> myGuard1(my_mutex1);

		//睡眠20s
		/*chrono::milliseconds dura(20000);
		this_thread::sleep_for(dura);*/

		if (!msgRecvQueue.empty())
		{
			//消息队列不为空
			command = msgRecvQueue.front();//返回第一个元素
			msgRecvQueue.pop_front();//移除第一个元素
			return true;
		}
		
		return false;
		//所有分支都要有unlock()，两个出口必须有两个unlock()

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
private:
	list<int>msgRecvQueue;//容器用来存放玩家发送过来的命令
	//创建一个互斥量的成员变量
	mutex my_mutex1;
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
 *unique_lock()取代lock_guard()
 *unique_lock是一个模板类，工作中一般推荐使用lock_guard()，其取代了lock(),unlock()函数
 *默认不带参数lock_guard和unique——lock相同
 *unique_lock支持更多的参数
 *adopt_lock:表示互斥量已经被lock了，互斥量必须在前面lock了，否则会报异常。
 *不需要在构造函数中再次lock了。
 *
 *try_to_lock()是我们会尝试用mutex的lock去锁定mutex，但是如果没有锁定成功，
 *会立即返回，不会等待锁住。(前面不能锁住)
 *
 *default_lock,不能先lock住，
 *就是没有给mutex加锁，初始化了一个没有加锁的mutex，可以灵活的调用一些unique_lock
 *的成员函数。
 *lock()
 * unlock()--会自动解锁（可能在中间处理一些非共享代码，然后再锁上）
 * try_lock()--尝试加锁，不成功也不阻塞
 * release()--返回它锁管理的mutex对象指针，并释放管理权
 *	也就是unique_lock和mutex不再有关系,如果原来mutex处于加锁状态，就需要自己接管解锁。
 *	不要混淆release()和unlock()
 * 用参数和使用成员函数都是可以的。
 *
 *为什么有时候需要unlock()，因为你lock锁住的代码越少，整个程序越快
 *有时候也把锁住的代码多少称为锁的粒度，粒度一般用粗细描述，锁住的代码越少，执行效率越高。
 *力度越粗，执行的效率越低。所以选择合适的粒度是程序员能力和实力的体现
 *
 *
 *unique_lock()所有权的传递，
 *通常情况下unique_lock应该和一个mutex相互关联。
 unique_lock<mutex> myGuard1(my_mutex1);
 *myGuard拥有my_mutex1的所有权，并且所有权可以转移给其他unique_lock对象
 *所有权不能复制。
 也可以通过函数返回
 *
*/

