#include <iostream>
#include <string>
#include<thread>

using namespace std;
/*
 * 线程id：id是一个数组，每一个主线程和子线程都对应着一个id数字，不同的线程
 * 他的线程id是不同的，线程id可以获取的，使用c++标准库中的函数获取thread::get_id Method
 * 
 */
class A
{
public:
	int m_i;
	//构造函数--类型转换构造函数，把一个整形转换为一个类A对象
	A(int a) :m_i(a)
	{
		cout << "A的构造函数执行" << endl;
		cout << this << "thread id" << this_thread::get_id() << endl;
	}
	//拷贝构造函数
	A(const A&m) :m_i(m.m_i)
	{
		cout << "A的拷贝构造函数执行" << endl;
		cout << this << "thread id" << this_thread::get_id() << endl;

	}
	~A()
	{
		cout << "A的析构函数执行" << std::endl;
		cout << this << "thread id" << this_thread::get_id() << endl;

	}
};


void MyPrint(const A&pMyBuf)
{
	cout << "子线程的MyPrint的参数地址是:" << &pMyBuf << endl;
	cout << "thread is:" << this_thread::get_id() << endl;
}

int main(void)
{
	int myVar = 10;
	int mySecondVar = 20;
	cout << "这是主函数的执行" << endl;
	cout << "主线程id是：" << this_thread::get_id() << endl;

	//thread mythread(MyPrint, myVar, mySecondVar);
	thread mythread(MyPrint, A(mySecondVar));

	//把一个整形对象MySecondVar传递给A类型对象的类型转换构造函数
	mythread.join();
	//mythread.detach();
	

	system("pause");
	return 0;
}

/*
 *测试可得，如果使用隐式类型转换，那么构造函数在子线程中执行
 *如果使用使用强制类型转换构造一个临时对象，那么会在主线程中构建对象。
 *只要在主线程中构造（拷贝构造）完毕临时对象，那么子线程就可以使用
 */