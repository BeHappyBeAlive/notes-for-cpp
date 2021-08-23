#include<iostream>
#include<thread>
#include<mutex>
#include<condition_variable> // ��������
#include<queue> // C++ STL���е������������̰߳�ȫ��
using namespace std;

/*
C++���̱߳�� - �̼߳��ͬ��ͨ�Ż���
���̱߳���������⣺
1. �̼߳�Ļ���
	��̬���� =�� �ٽ�������� =�� ԭ�Ӳ��� =�� �̻߳�����mutex ������������ʵ��CAS
	strace ./a.out mutex => pthread_mutex_t
2. �̼߳��ͬ��ͨ��
	�����ߡ��������߳�ģ��

*/
std::mutex mtx; // ���廥����
std::condition_variable cv; //�����������������̼߳��ͬ��ͨ�Ų���

// ����������һ����Ʒ��֪ͨ����������һ������������֮��������֪ͨ�����߼�������
class Queue {
public:
	void put(int val) { // ������Ʒ
		//lock_guard<std::mutex> guard(mtx); //scoped_ptr
		unique_lock<std::mutex> lck(mtx); // unique_ptr
		while (!que.empty()) { 
			// que��Ϊ�գ�������Ӧ��֪ͨ������ȥ���ѣ���������������
			// �������߳�Ӧ�ý�������#1�ȴ�״̬������#2��mtx�������ͷŵ�
			
			cv.wait(lck);
		}
		que.push(val);
		/*
			notify_one:֪ͨ����һ���߳�
			notify_all:֪ͨ���������߳�
			֪ͨ�����߳���������һ����Ʒ���Ͽ�����
			�����̵߳õ���֪ͨ���ͻ�ӵȴ�״̬ =�� ����״̬ =�� ��ȡ���������ܼ���ִ��
		*/
		cv.notify_all(); 
		cout << "������ ������" << val << "����Ʒ" << endl;
	}
	int get() { // ������Ʒ
		//lock_guard<std::mutex> guard(mtx);
		unique_lock<std::mutex> lck(mtx);
		while (que.empty()) {
			//�����߷���queΪ�գ�֪ͨ�������߳���������Ʒ
			//#1 ����ȴ�״̬ #2 �ѻ�����mutex�ͷŵ�
			
			cv.wait(lck);
		}
		int val = que.front();
		que.pop();
		cv.notify_all(); //֪ͨ�����߳����������ˣ��Ͽ�������
		cout << "������ ���ѣ�" << val << "����Ʒ" << endl;
		return val;
	}
private:
	queue<int> que;
};

void producer(Queue *que) //�������߳�
{
	for (int i = 1; i <= 10; ++i) 
	{
		que->put(i);
		std::this_thread::sleep_for(std::chrono::milliseconds(100));
	}

}

void consumer(Queue* que) //�������߳�
{
	for (int i = 1; i <= 10; ++i)
	{
		que->get();
		std::this_thread::sleep_for(std::chrono::milliseconds(100));
	}

}

int main() {
	Queue que; // �����̹߳���Ķ���

	std::thread t1(producer, &que);
	std::thread t2(consumer, &que);

	t1.join();
	t2.join();
}
