#include<iostream>
#include<thread>
#include<mutex>
#include<condition_variable> // 条件变量
#include<queue> // C++ STL所有的容器都不是线程安全的
using namespace std;
#if 0
/*
unique_lock需要配合condition_variable使用
1. lock_guard和unique_lock
2. condition_variable  wait和notify_all方法
*/
std::mutex mtx;
std::condition_variable cv;
int main() {

	/*
	通知在cv上等待的线程，条件成立，可以起来干活了
	其他cv上等待的线程，收到通知，从等待状态=》阻塞状态=》拿到互斥锁=》线程继续运行
	*/
	cv.notify_all();



	//不仅可以运用在简单的临界区代码段的互斥操作中，还能再函数调用过程中
	unique_lock<std::mutex> lck(mtx);
	//#1 使线程进入等待状态 #2 将互斥锁mtx释放掉，因为其中调用了lck.unlock()
	cv.wait(lck);
	//出作用域就作用消失
	//因为拷贝构造和赋值重载都delete了，所以不能用在函数参数传递或者返回过程中，
	//只能用在简单的临界区代码段的互斥操作中，这是类似于unique_lock
	//lock_guard<std::mutex> guard(mtx);

	//简单的互斥锁
	//mtx.lock();

	//mtx.unlock();

	return 0;
}
#endif
