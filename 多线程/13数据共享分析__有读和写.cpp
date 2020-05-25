#include <iostream>
#include <string>
#include<thread>
#include <vector>
#include<list>
using namespace std;

//用成员函数作为线程初始化函数

class A
{
public:
	//把收到的消息入到一个队列，子线程的启动函数
	void inMsgRecvQueue()
	{
		for (int i = 0; i < 10000;i++)
		{
			cout << "inMsgQueue插入一个元素" << i << endl;
			msgRecvQueue.push_back(i);//假设这个数字i就是收到的玩家的命令
		}
	}

	//把数据从消息队列中取出的子线程
	void outMsgRecvQueue()
	{
		for (int i = 0; i < 10000;i++)
		{
			if (!msgRecvQueue.empty())
			{
				//消息队列不为空
				int command = msgRecvQueue.front();//返回第一个元素
				msgRecvQueue.pop_front();//移除第一个元素
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
*有的线程写数据，有的线程写数据，那么需要特殊的处理,
*最简单的处理就是读的时候不能写，写的时候不能读，只能有一个线程写
*也就是只能有一个线程占有数据。向容器中写数据和读数据都是分很多步骤
*如：移动指针等。由于任务切换会导致很多诡异的事情发生。
*
*
*其他情况：
*数据共享的实例；火车票的剩余票数就需要在所有的客户端进行共享，并且操作之间
*要原子性的 
*
*共享数据的保护案例代码
*假设要做一个网络游戏服务器，有两个子线程，一个线程收集玩家命令，并把命令写进一个队列中
*第二个线程从队列中取出玩家发出的命令，解析执行玩家的动作。
*这里用数字表示玩家发送过来的命令，使用list保存。在频繁的按顺序和取数据效率高--双向列表容器
*
*当有读和有写的两个线程，必须进行控制，保证只有一个线程进行读或者写即可。
*引入：互斥量问题--多线程共享数据的保护问题
*
*/

