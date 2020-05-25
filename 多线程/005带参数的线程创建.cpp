#include <iostream>
#include <string>
#include<thread>

using namespace std;



//线程函数
//void MyPrint(int&i,char*pMyBuf)
void MyPrint(int&i,const string&pMyBuf)
//这里可以使用引用,问题是MyBuf到底是在什么时候转换为string的？？
//确实存在当mian()函数执行功能完毕，才会把字符数组MyBuf转换为string
{
	cout << i << endl;
	cout << pMyBuf << endl;
	return;
}

int main(void)
{
	//传递临时对象作为参数详解
	int  myvar = 10;
	int& myvarReference = myvar;
	char mybuf[] = "这是测试字符串";
	//第一个参数是线程的执行函数，剩下的是多个参数列表
	//thread myThread(MyPrint, myvar, mybuf);
	thread myThread(MyPrint, myvar, string(mybuf));
	//这里把字符数组MyBuf对象转换为临时的string对象，再进行参数传递

	//myThread.join();
	//当主线程和子线程分类的时候，传递的参数int&i是引用（会复制），会不会出错
	//不会，写的参数虽然是引用，但是会复制一份.但是第二个指针参数是有问题的
	//使用指针默认执行的是浅拷贝
	myThread.detach();
	cout << "这是主线程的执行" << endl;

	system("pause");
	return 0;
}

