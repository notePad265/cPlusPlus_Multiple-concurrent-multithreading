#include <iostream>
#include <string>
#include<thread>
using namespace std;
class A
{
public:
	int m_i;
	//构造函数--类型转换构造函数，把一个整形转换为一个类A对象
	A(int a) :m_i(a)
	{
		cout << "A的构造函数执行" << endl;
	}
	//拷贝构造函数
	A(const A&m) :m_i(m.m_i)
	{
		cout << "A的拷贝构造函数执行" << endl;
	}
	~A()
	{
		cout << "A的析构函数执行" << std::endl;
	}
};


void MyPrint(const int i, const A&pMyBuf)
{
	cout << &pMyBuf << endl;//打印的是地址
	return;
}

int main(void)
{
	int myVar = 10;
	int mySecondVar = 20;

	//thread mythread(MyPrint, myVar, mySecondVar);
	thread mythread(MyPrint, myVar, A(mySecondVar));

	//把一个整形对象MySecondVar传递给A类型对象的类型转换构造函数
	//mythread.join();
	mythread.detach();
	cout << "这是主函数的执行" << endl;
	system("pause");
	return 0;
}
/*
 * 当main()函数执行完毕，使用mySecondVar来构造一个类对象，这种写法有问题
 * 那么我在传递参数的时候是否可以构造一个临时对象的时候，时候可以解决问题？？
 * 能解决问题，会执行一个拷贝构造函数，也就是在主函数有效的时候已经构造一个A
 * 对象传递给线程的参数中，从此不再依赖于主函数中的对象。
 * 
 * 在创建线程的时候构造临时对象传递参数的方法是可行的。
 void MyPrint(const int i, const A&pMyBuf)
 * 实际上线程的参数对象使用引用也会额外复制出一个对象，而不是使用引用
 * 
 * 本质要求：（1）只要把临时对象构造出来传递给线程，那么就能在主线程执行完毕后，子线
 * 程也是安全的。
 * （2）用隐式类型转换是不安全的，构造临时对象是安全的。
 * 
 * （3）若传递int这种简单类型，建议都是值传递，不要使用引用传递
 * （4）如果传递类对象，避免隐式类型准换，（char*--->string）,应该在创建线程这一行
 *	构建出临时对象来，在函数的形式参数里面使用引用来进行接受，否则系统还会构造出来一次对象
 *	
 *	
 *	所作的工作都是预防主线程的退出导致子线程的非法引用。
 *	终极结论：建议不适用detach() ，只使用join()，这样就不存在局部变量失效使引用失效的问题
 *	
 *	思考：为什么使用临时对象就安全了，而使用自动类型转换就是不安全的？？
 *	
 *	
 */



