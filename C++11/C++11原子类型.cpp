#include<iostream>
#include<thread>
#include<atomic> //包含了很多原子类型
#include<list>
using namespace std;

/*
C++11
窗口卖票 int count = 100;
lock_guard<std::mutex> guard(mtx);
count++;

lock_guard<std::mutex> guard(mtx);
count--;

互斥锁比较重，临界区代码做的事情稍微有点复杂，很繁多
可以采用CAS来保证上面对于count的++ -- 操作的原子特性就足够了，无锁操作
		通过CPU硬件总线中的exchange/swap来实现无锁操作 
		一般无锁队列 =》CAS来实现的
*/
//防止多线程对共享变量的缓存，使得所有线程访问的都是原始内存中的值
volatile std::atomic_bool isReady = false;
volatile std::atomic_int mycount = 0;

void task() {
	while (!isReady) {
		std::this_thread::yield();//线程让出当前的CPU时间片，等待下一次调度
	}
	for (int i = 0; i < 100; ++i) {
		mycount++;
	}
}


int main() {
	list<std::thread> tlist;
	for (int i = 1; i <= 10; ++i) {
		tlist.push_back(std::thread(task));
	}

	std::this_thread::sleep_for(std::chrono::seconds(3));
	isReady = true;

	for (std::thread& t : tlist) {
		t.join();
	}

	cout << "count:" << mycount << endl;

	return 0;
}