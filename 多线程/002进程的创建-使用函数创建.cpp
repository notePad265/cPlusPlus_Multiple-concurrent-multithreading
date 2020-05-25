#include <iostream>
#include <string>
#include <thread>
using namespace std;

//线程的初始函数--从这个函数开始执行
void MyPrint()
{
	cout << "子线程开始执行1\n" << endl;

	cout << "子线程执行完毕2\n" << endl;
	cout << "子线程执行完毕3\n" << endl;
	cout << "子线程执行完毕4\n" << endl;
	cout << "子线程执行完毕5\n" << endl;
	cout << "子线程执行完毕6\n" << endl;

}

int main(void)
{
	//实际上是主线程在执行(自动创建的主线程)，主线程main()函数返回，则整个进程执行完毕
	


	//多线程main()中的部分
	thread my_thread(MyPrint);//（1）创建了线程，线程的起点（入口）是MyPrint（2）MyPrint标志的线程已经开始执行，主线程和MyPrint（）并行执行 
	//An application-defined function to be executed by the thread.
	//让主线程阻塞等待子线程执行完毕，然后主线程和子线程会合，然后主线程再继续执行。
	//my_thread.join();//阻塞，直到执行完毕Blocks until the associated thread completes.
	if (my_thread.joinable())
	{
		cout << "can join" << endl;
	}
	else
	{
		cout << "can not join" << endl;
	}
	my_thread.detach();

	if (my_thread.joinable())
	{
		cout << "2can join" << endl;
	}
	else
	{
		cout << "2can not join" << endl;
	}
	//主线程阻塞到这里等待MyPrint执行完毕，当子线程执行完毕，就解阻塞，主线程继续执行
	//去掉这一行主线程和子线程交替执行，会输出混乱，当主线程先结束的时候，子线程可能未执行完毕，会被操作系统杀死
	//一个书写良好的程序，应该是主线程等待子线程执行完毕后，自己才能结束

	//joinable()用于判断时候可以使用join或者detach  --Specifies whether the associated thread is joinable.返回true或者false



	/*
	 *自己创建一个线程，主线程从main()函数开始执行，自己创建的线程也需要从一个函数开始执行
	 *初始函数--一旦执行完毕，就代表着这个线程结束。
	 *
	 *整个进程是否执行完毕的标志是主线程是否执行完毕，如果主线程执行完毕，就代表整个进程结束。
	 *此时如果子线程没有执行完毕，那么这些子线程也会被操作系统强行终止。
	 *所以一般情况下我们得到一个结论：要想保证子线程（自己用代码创建的线程）的运行状态就要让主线程一直保持运行，
	 *不要主线程运行完毕
	 *例外情况：后续会学习，目前这样记忆即可。
	 *
	 *怎么写多线程程序
	 *	（1）包含头文件thread  #include <thread>
	 *	 (2)创建一个函数作为子线程开始执行的地方--初始函数
	 *	 （3）在main()函数中写代码，创建一个子线程
	 *	 （4）当子函数的代码执行完毕，子线程退出程序
	 *现在必须明白这个代码有两个线程在跑，相当于这个程序有两个线在同时跑，可以同时做两件事情
	 *即使一条线被堵住了，另外一条线还是可以通行的。
	 *
	 */

	/*
	 * 主线程是不是可以不等待子线程执行完毕，自己先结束，可以detache()Detaches the associated thread from the thread object.
	 The operating system becomes responsible for releasing thread resources on termination.
	 * 传统的多线程程序子线程要等待子线程执行完毕，然后自己退出，使用detach可以让主线程和子线程分离分离执行。
	 * detach()会导致线程失去我们自己的控制。一旦调用detach()线程就被操作系统接管，就不能再使用join()，否则会有异常
	 * 
	 * 
	 * 为什么引入detach()
	 *  （1）当我们创建了很多子线程，让主线程一个一个等待子线程结束，所以引入detach()
	 *   (2)一旦tetach()之后，与这个主线程关联的thread对象就会失去与主线程的关联。
	 *		这个子程序就会驻留在后台运行，这个子程序就被操作系统接管，当子线程运行完成后
	 *		由运行时库（操作系统）清理相关的资源。在linux下就是守护线程。
	 *		主线程退出会导致子线程的输出转入到后台进行
	 * 
	 */
		cout << "这是主线程在执行\n" << endl;
	



	system("pause");
	return 0;
}

/*
 * 总结
 * （1）必须子线程执行完毕，主线程再执行完，也就说主线程必须阻塞等待子线程执行完毕，然后在结束
 */