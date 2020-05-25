#include <iostream>
#include <string>
#include<thread>

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


	void thread_work(int num)
	{
		cout << "子线程中的成员函数" << endl;
	}

	void operator()(int num)
	{
		cout << "使用类对象创建线程（）" << endl;
	}
};


int main(void)
{
	A obj(1100);
	//传递的线程的函数需要取地址，并且加上类作用域，对象，形式参数的值
	//thread mythread(&A::thread_work, obj, 14);
	//如果真的传递一个引用，则拷贝构造函数不会执行
	thread mythread(&A::thread_work, ref(obj), 14);
	//thread mythread(obj, 14);
	//thread mythread(ref(obj), 14);

	mythread.join();
	system("pause");
	return 0;
}

//使用类的成员函数执行一个线程
