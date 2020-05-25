#include <iostream>
#include <string>
#include<thread>
#include <future>
using namespace std;

vector<std::packaged_task<int(int)>> myTasks;//t容器

int main(void)
{
	cout << "主线程id=" << std::this_thread::get_id() << endl;
	std::packaged_task<int(int)>mpt([](int mypar)
	{
		cout << "mpar=" << mypar << endl;
		chrono::milliseconds dura(5000);
		this_thread::sleep_for(dura);
		cout << "线程开始执行,线程的id是：" << std::this_thread::get_id() << endl;
		//休息5s

		return 5884;
	});
	std::thread t1(std::ref(mpt), 1);//1是传递的参数,创建一个线程，并开始执行
	t1.join();
	//packaged_task把线程函数包装了一层

	//通过future获取未来函数执行的值
	//std::future对象里面包含有线程入口函数的结果，result包含mythread的返回结
	//get_future()是packaged_task()对象的接口
	std::future<int> result = mpt.get_future();
	cout << result.get() << endl;

	////////////////////////////////////////////////////////////////////////////
	myTasks.push_back(std::move(mpt));
	//复制
	std::packaged_task<int(int)>mypt2;
	auto iter = myTasks.begin();
	mypt2 = std::move(*iter);//移动语义
	myTasks.erase(iter);//删除第一个元素，迭代器已经失效，不能在继续使用
	mypt2(123);
	std::future<int> result2 = mypt2.get_future();
	cout << result2.get() << endl;


	system("pause");
	return 0;
}

/*
*packaged_task 打包任务，把任务包装起来
*是个类模板，它的模板参数是各种可调用对象，通过packageed_task可以把
*各种可调用对象包装起来，方便作为线程入口函数来调用。
*packaged_task包装起来的可调用对象，还可以直接调用，从这个角度来讲packaged_task也是一个
*可调用对象。
*/

