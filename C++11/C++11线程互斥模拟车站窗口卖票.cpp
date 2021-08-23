#include<iostream>
#include<thread>
#include<list>
#include<mutex>
using namespace std;

#if 0
/*
C++ thread 模拟车站三个窗口卖票的程序

*/

int Ticketcount = 100; // 车站有100张车票，由三个窗口一起卖票
std::mutex mtx; // 全局的一把互斥锁

/*
多线程程序
竞态条件：多线程程序执行的结果是一致的，
不会随着CPU对线程不同的调用顺序，而产生不同的运行结果

如果出现运行结果每次不同，那么说明该程序中存在竞态条件，所以存在线程安全问题
*/
//模拟卖票的线程函数 lock_guard unique_lock
void sellTicket(int index) 
{
	//mtx.lock();
	//这样的结果只是一个窗口在卖，所以需要在临界区代码段加锁，而不是这里
	while (Ticketcount > 0) {
		//mtx.lock();
		{
			// 出局部作用域就析构掉
			lock_guard<std::mutex> lock(mtx); // 保证所有线程都能释放锁，防止死锁的问题发生 
			// 临界区代码段  原子操作 线程互斥mutex
			// 如果现在还剩一张票，线程1进入循环，线程1拿到锁
			// 那么假设线程1还未执行到--操作，这时候线程2进入到循环，在锁的位置阻塞
			// 那么当线程1执行完票剩余为0，这个时候线程2拿到锁，执行，票剩余为-1，
			// 则这样就会出错
			// 所以一般在加锁的里面会再次加上一个判断，再次判断临界资源是否不满足要求了
			if (Ticketcount > 0) {
				cout << "窗口：" << index << "卖出第：" << Ticketcount << "张票" << endl;
				Ticketcount--;
			}
		}
		//mtx.unlock();

		std::this_thread::sleep_for(std::chrono::milliseconds(100));
	}
	//mtx.unlock();
}


int main() {
	list<std::thread> tlist;
	for (int i = 1; i <= 3; ++i) {
		tlist.push_back(std::thread(sellTicket,i));
	}
	for (std::thread &t : tlist) {
		t.join();
	}
	return 0;
}
#endif