#include <iostream>
#include <string>
#include<thread>
#include <vector>

using namespace std;

//共享数据
vector<int> g_var = { 1, 2, 3 };
//线程入口函数
void MyPrint(int num)
{
	cout << "id ="<<this_thread::get_id()<<"的值"<< g_var[0] << endl;
	return;
}


int main(void)
{
	vector<thread> mythread;
	for (int i = 0; i < 10;i++)
	{
		mythread.push_back(thread(MyPrint, i));
	}

	for (auto iter = mythread.begin(); iter != mythread.end();iter++)
	{
		iter->join();
	}

	cout << "main线程" << endl;//最后执行这一句，整个线程退出
	system("pause");
	return 0;
}
/*
 *多个数据是只读的，每个线程都去读数据，是安全稳定的，不需要什么特别的手段
*/

