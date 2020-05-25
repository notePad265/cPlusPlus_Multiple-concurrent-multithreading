#include <iostream>
#include <string>
#include <thread>

using namespace std;

int main(void)
{

	//使用lambda表达式创建线程
	auto myLambdaThread = []
	{
		cout << "子进程开始执行" << endl;
		cout << "子进程结束执行" << endl;

	};

	thread mythread(myLambdaThread);
	mythread.join();
	
	cout << "这是主函数执行" << endl;
	system("pause");
	return 0;
}
/*

*/