#include <iostream>
#include <string>
#include<thread>
#include <future>

using namespace std;


int  mythread()
{
	cout << "子线程id=" << std::this_thread::get_id() << endl;
	return 1;
}


int main(void)
{
	cout << "main thread id =" << std::this_thread::get_id() << endl;
	//创建一个异步线程
	//std::future<int> result = std::async(mythread);

	std::future<int> result = std::async(mythread);
	std::future_status status = result.wait_for(std::chrono::seconds(0));//是否被立即执行了
	if (status==std::future_status::ready)//--timeout --
	{
		cout << "线程执行完毕" << endl;
	}
	else if (status == std::future_status::deferred)
	{
		cout << "线程被延迟调用" << endl;
	}
	else if (status == std::future_status::timeout)
	{
		cout << "线程超时间，还没有执行结束" << endl;
	}
	cout << result.get() << endl;
	system("pause");
	return 0;
}

/*
 *std::async参数详细谈
 *--用来创建一个异步任务
 *参数--延迟调用参数
 *std::async(std::launch::deferred)--延迟调用，并且不创建新线程--延迟到future对象调用get(),wait()方法才会创建
	std::async(std::launch::async);--强制创建一个异步任务（线程）,在新线程中执行
	std::async(std::launch::any);
	std::async(std::launch::sync);
	deferred|async使用亦或，则由操作系统自己评估选择，（1创建新线程，立即执行）或者（延迟到get,wait方法执行），是两个行为中的一种
	不带参数--其实没有指定参数，默认参数是（deferred|async），上一课讲错了
自行决定是什么意思？？怎么决定是否创建新线程？？
|
|
|	async()--我们一般叫创建一个异步任务，async()有时候不创建一个新线程，谁调用get()方法，就在那个西安测绘给你中执行
|
v
std::thread()如果系统资源匮乏，那么可能你创建线程失败。导致整个程序崩溃
thread()创建的返回值不太好接收，要使用一个全局量等手段接受。
std::async()创建异步任务（可能创建线程，也可能不创建线程），线程的返回值可以使用future对象来接受
用async()创建线程一般不会报异常，如果系统资源紧张，不加额外参数的调用async()就不会创建线程，
而是后续谁调用get()来请求结果，线程入口函数就运行在调用的线程中。
如果强制创建新线程，就要使用std::async(std::launch::async);


根据经验，一个程序中线程数目在100-200，线程切换会浪费很多cpu资源。
std::async()不确定性问题解决
	不加参数的async()调用，不能确定系统能否创建出新线程。如果系统自主选择了推迟运行
	意味着mythread线程入口函数不会马上执行，后面如果没有调用get()，压根就不执行。
	这种写法deferred|async是否会推迟创建线程。要使用std::wait_for()
*/