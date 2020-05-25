#include <iostream>
#include <string>
#include<thread>
#include <future>
using namespace std;

int mythread()
{
	chrono::milliseconds dura(5000);
	this_thread::sleep_for(dura);
	cout << "线程开始执行,线程的id是：" << std::this_thread::get_id() << endl;
	//休息5s

	return 5884;
}

int mythread2()
{
	chrono::milliseconds dura(5000);
	this_thread::sleep_for(dura);
	cout << "线程开始执行,线程的id是：" << std::this_thread::get_id() << endl;
	//休息5s

	return 5884;
}
int main(void)
{
	cout << "main thread id:" << std::this_thread::get_id() << endl;
	std::future<int> result = std::async(mythread);
	//std::future<int> result = std::async(std::launch::deferred,mythread);

	//自动创建一个线程，并启动线程入口函数
	cout << "continue........" << endl;

	
	
	
	//int def = 0;
	////卡在这里等待线程执行完毕，通过future对象的get（）方法等待程序结束并得到返回结果
	//cout << "未来的执行结果是：" << result.get() << endl;

	//枚举类型
	//wait_for()等待一定时间
	std::future_status status = result.wait_for(std::chrono::seconds(6));//等待1s
	if (status==std::future_status::timeout)//超时表示线程还没有执行完毕
	{
		cout << "超时，线程还没有执行完毕" << endl;
	}
	else if (status==std::future_status::ready)
	{
		cout << "线程执行完毕" << endl;
	}
	else if (status == std::future_status::deferred)//延迟
	{
		//如果async()的第一个参数被设置为deferred ,则本条件成立
		cout << "线程被延迟执行" << endl;
		//使用get()才会执行，且只能get一次
		cout << result.get() << endl;
	}


	//创建第二个线程	

	std::future<int> result2 = std::async(mythread2);
	std::shared_future<int>ressult3(std::move(result2));
	////判断future对象里面是否有值
	//bool ifCanGet = ressult3.valid();
	//cout << ifCanGet << endl;//结果为1
	//std::shared_future<int>ressult3(result2.share());//等价于这种写法
	//这里可以get多次
	auto mythreadResult = ressult3.get();

	system("pause");
	return 0;
}

/*
 *wait_for()返回是一个枚举类型
 *get()方法使用两次会产生异常，因为get()方法的设计是一个移动语义。
 *share_future()也是一个类模板，future对象只能get()一次，share_future()的get()
 *方法是复制数据。
*/

