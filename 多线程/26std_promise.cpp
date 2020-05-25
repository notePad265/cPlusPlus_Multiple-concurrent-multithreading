
#include <iostream>
#include <string>
#include<thread>
#include <future>

using namespace std;

void mythread(std::promise<int>&tem,int calc)
{
	calc++;
	calc *= 10;
	chrono::milliseconds dura(5000);
	std::this_thread::sleep_for(dura);
	//计算整整花费了5秒中

	int result = calc;
	//把结果传递出来
	tem.set_value(result);
}

void mythread2(std::future<int>&tem)
{
	auto result = tem.get();
	cout << "mythread2 result=" << endl;
	return;
}

int main(void)
{
	std::promise<int> mypromise;//生命一个promise对象，保存类型为int
	thread t1(mythread, std::ref(mypromise), 120);
	t1.join();

	//获取结果
	//绑定
	std::future<int>future1 = mypromise.get_future();
	auto result = future1.get();//get()只能调用一次，不能调用多次
	cout << result << endl;

	//在其他线程中使用
	thread t2(mythread2, std::ref(future1));
	t2.join();

	system("pause");
	return 0;
}


/*
 * std::promise也是一个类模板
 * 我们在某一个线程中给他赋值，然后再其他线程中把这个值提取出来。
 * 通过promise保存一个值，在将来某个时刻我们通过把一个future对象绑定到这个promiese来
 * 得到绑定的值
 */