#include<iostream>
#include<thread>
using namespace std;

#if 0
// thread的创建和一些基本操作
/*
C++语言级别的多线程编程 =》 代码可以跨平台使用
thread/mutex/condition_variable
lock_guard/unique_lock
atomic 原子类型 基于CAS操作的原子类型 是线程安全的
睡眠sleep_for

C++语言层面 thread
	windows				linux
		|				  |
	cretateThread	 pthread_create
*/
/*
线程内容：
1. 怎么创建启动一个线程
std::thread定义一个线程对象，传入线程所需要的线程函数和参数，
线程自动开启执行
2. 子线程如何结束
子线程函数运行完成线程就结束了

3. 主线程如何处理子线程
t1.join :  等待t1线程结束，当前线程继续往下运行
detach : 把t1线程设置为分离线程，主线程结束，整个进程结束，所有子线程都自动结束了，
		类似于守护线程，当前进程结束，守护线程全部结束
*/
void threadHandle1(int time)
{
	//让子线程睡眠两秒
	std::this_thread::sleep_for(std::chrono::seconds(time));
	cout << "call threadHandle1()" << endl;
}
int main() {
	//定义了一个线程对象 传入了一个线程函数 新线程就开始运行
	std::thread t1(threadHandle1, 2);
	// 主线程等待子线程结束，主线程才继续往下运行
	//t1.join();
	// 主线程必须结束，否则会抛出异常，如果想要避免这种情况
	// 可以将子线程设置为分离线程
	t1.detach();
	cout << "main thread done!" << endl;
	return 0;
}
#endif