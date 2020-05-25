#include <iostream>
#include <string>
#include<thread>
#include <memory>

using namespace std;

class A
{
public:
	mutable int m_i;//m_i的值是可以修改的，即使在const中
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
	pMyBuf.m_i = 3333;//我们修改这个值，不会影响到主线程中的值
	cout << "子线程的MyPrint的参数地址是:" << &pMyBuf << endl;
	cout << "thread is:" << this_thread::get_id() << endl;
}

//传递一个智能指针
void MyPrint2(unique_ptr<int>pint)
{
	cout << "子线程thread id is:" << this_thread::get_id() << endl;

}

int main(void)
{
	A myobj(10);
   //thread mythread(MyPrint,myobj);//将类对象作为线程的参数
	thread mythread(MyPrint, ref(myobj));//将类对象作为线程的参数




	
	mythread.join();
	//mythread.detach();

	system("pause");
	return 0;
}

/*
 *传递类对象，智能指针作为线程参数传递
 *std::ref()函数
 *虽然用引用作为参数，但是依然调用拷贝构造函数，向子线程传递类对象类型的参数，不管接收者
 *使用什么类型接受（引用接受还是值接受），编译器都是按照拷贝的形式接受。
 *当我们需要传递一个影响主线程的引用，而不是拷贝，那么使用ref（reference）,就不会调用拷贝构造函数
 */