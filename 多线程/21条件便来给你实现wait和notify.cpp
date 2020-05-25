#include <iostream>
#include <string>
#include<thread>
#include <vector>
#include<list>
#include<mutex>
#include <condition_variable>//需要包含这个，否则会报错类型不完整
using namespace std;

//用成员函数作为线程初始化函数

class A
{
public:

	//把收到的消息入到一个队列，子线程的启动函数
	void inMsgRecvQueue()
	{
		for (int i = 0; i < 10000; i++)
		{
			cout << "inMsgQueue插入一个元素" << i << endl;

			unique_lock<mutex> myGuard1(my_mutex1);
			msgRecvQueue.push_back(i);
			//my_cond.notify_one();//我们尝试把wait线程唤醒，执行后，wait被唤醒（本身解锁）
			my_cond.notify_all();
		}
	}
	//读共享数据函数的封装函数
	//bool outMsgprocess(int&command)
	//{
	//	//添加双重检查---思考：能否改为当数据不为空的时候，通知我们
	//	if (!msgRecvQueue.empty())
	//	{
	//		unique_lock<mutex> myGuard1(my_mutex1);
	//		if (!msgRecvQueue.empty())
	//		{
	//			//消息队列不为空
	//			command = msgRecvQueue.front();//返回第一个元素
	//			msgRecvQueue.pop_front();//移除第一个元素
	//			return true;
	//		}
	//	}
	//	

	//	return false;
	//	//所有分支都要有unlock()，两个出口必须有两个unlock()
	//}

public:
	//把数据从消息队列中取出的子线程
	void outMsgRecvQueue()
	{
		int command = 0;
		while (true)
		{
			unique_lock<mutex>sbguard1(my_mutex1);
			my_cond.wait(sbguard1,[this]
			{
				if (!msgRecvQueue.empty())
				{
					return true;
				}
				return  false;
			});
			command = msgRecvQueue.front();
			msgRecvQueue.pop_front();
			sbguard1.unlock();//因为unique_lock的灵活性，可以随时解锁
			//下面是一些具体的动作，比如玩家抽卡等操作，，，，
			//加入现在正在处理一个事务，需要一段时间，而不是卡在wait()等待，此时这个
			//notify_one()函数并没有作用。也就是说notify_one()不一定可以唤醒wait（）
		}
		/*
		 * wait()用来等待一个东西
		 * 第二个参数lambad表达式返回值是false，那么wait将解锁互斥量。并阻塞到本行（睡眠），
		 * 阻塞到其他函数调用notify_one()为止。没有第二个参数等价与false。
		 * 
		 * 如果第二个返回值是true，，那么wait直接返回
		 * 当其他线程使用notefy_one()唤醒wait再次尝试获取互斥量锁，如果获取不到线程
		 * 阻塞这里等待获取，如果获取到锁，wait就继续执行b
		 * （b）上锁
		 *		如果wait有第二个参数，就（lambda），就判断这个表达式，如果表达式为false，就将互斥量解锁
		 *		重新进入睡眠，等待再次被notefy_one()唤醒
		 *		如果表示式为true，那么流程直接走下来。此时互斥锁还是锁着。
		 *		流程只要能走到这里互斥锁一定是锁着的。
		 *	如果wait没有第二个参数无条件向下走。
		 * 
		 * 
		 */
		
	}

		/*cout << endl;*/
private:
	list<int>msgRecvQueue;//容器用来存放玩家发送过来的命令
	//创建一个互斥量的成员变量
	std::mutex my_mutex1;
	std::condition_variable my_cond;//2017可以不适用()
	//生成一个条件对象
};


int main(void)
{
	A myobj;
	//测试哪一个线程被唤醒
	thread myOutMsgObj(&A::outMsgRecvQueue, &myobj);//第二个是引用才能保证线程中用的是同一个对象
	thread myOutMsgObj2(&A::outMsgRecvQueue, &myobj);

	thread myInMsObj(&A::inMsgRecvQueue, &myobj);
	myOutMsgObj.join();
	myInMsObj.join();
	myOutMsgObj2.join();

	cout << "main线程" << endl;//最后执行这一句，整个线程退出
	system("pause");
	return 0;
}
/*
 *条件变量 std::condition_variable 
 *条件变量--用在线程中，当线程A等待条件满足，线程B专门网消息队列中扔消息
 *是一个和条件相关的类，说白了就是等待条件满足，需要和互斥量配合使用
 *
 *
 *wait  ()--需要创建一个条件变量，睡眠进行
 *notify_one()--唤醒wait()进程，但是不能保证一定能唤醒
 *
 *深入思考：
 *当两个线程拿到锁的次数不一样，也就说数据处理不过来怎么办
 *		开更多的线程或者约束数据的次数
 *		实际的环境中，如果玩家的操作一直得不到执行是不可以的。
 *
 *很多公司喜欢要年轻人，年轻人能加班，对于简单的问题，用年轻人没问题。
 *越是困难的问题，越是要用老人，经验丰富的问题，可以把问题消失在萌芽之中。
 *重要岗位一定要使用经验丰富的人
 *
 *notify_all()函数
 *		notify_one()只能通知一个线程,要想通知多个线程就需要使用notify_one()
*/

