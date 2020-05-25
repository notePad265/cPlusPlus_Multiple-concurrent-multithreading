#include <iostream>
#include <string>
#include <thread>

using namespace std;
class testClass
{
public:
	int &m_i;
	//构造函数
	testClass(int&i) :m_i(i)
	{
		cout << "类testClass的构造函数被执行" << endl;
	}

	//拷贝构造函数
	testClass(const testClass&ts):m_i(ts.m_i)
	{

		cout << "拷贝构造函数被执行" << endl;
	}

	~testClass()
	{
		cout << "析构函数被执行" << endl;
	}

	void operator()()//重载()运算符，变成一个可调用对象
	{
		cout << "我的线程opertator开始执行了\n" << endl;
		cout << "m_i1=" << m_i << endl;
		cout << "m_i2=" << m_i << endl;
		cout << "m_i3=" << m_i << endl;
		cout << "m_i4=" << m_i << endl;
		cout << "我的线程opertator结束执行了\n" << endl;

	}
};

int main(void)
{

	//使用类创建线程
	int myi = 6;
	testClass mytestclass(myi);
	thread myThread(mytestclass);//使用类对象创建一个线程

	//myThread.join();
	myThread.detach();
	cout << "这是主线程的执行\n" << endl;

	system("pause");
	return 0;
}
/*
 * 子线程打印输出主线程的一个int类型的空间，当主线程执行完毕，子线程继续访问主线程的
 * 内存空间，就会出错。所以这里不能使用引用。
 * 问题：当主线程执行完毕，那么主线程中的定义的类对象还能使用吗？主线程中的对象没有被销毁吗？？
 *	原因是：对象已经不存在了，但是复制了一个新对象mytestclass到子线程中。主线程中的对象不存在了。
 *	但是复制的对象依旧存在。只要类里面没有引用，指针等就不会有问题。会调用拷贝构造函数。但是使用detach()
 *	构造和析构不能控制，使用join()可以控制。
 *	
 */