#include<iostream>
#include<thread>
#include<list>
#include<mutex>
using namespace std;

#if 0
/*
C++ thread ģ�⳵վ����������Ʊ�ĳ���

*/

int Ticketcount = 100; // ��վ��100�ų�Ʊ������������һ����Ʊ
std::mutex mtx; // ȫ�ֵ�һ�ѻ�����

/*
���̳߳���
��̬���������̳߳���ִ�еĽ����һ�µģ�
��������CPU���̲߳�ͬ�ĵ���˳�򣬶�������ͬ�����н��

����������н��ÿ�β�ͬ����ô˵���ó����д��ھ�̬���������Դ����̰߳�ȫ����
*/
//ģ����Ʊ���̺߳��� lock_guard unique_lock
void sellTicket(int index) 
{
	//mtx.lock();
	//�����Ľ��ֻ��һ������������������Ҫ���ٽ�������μ���������������
	while (Ticketcount > 0) {
		//mtx.lock();
		{
			// ���ֲ��������������
			lock_guard<std::mutex> lock(mtx); // ��֤�����̶߳����ͷ�������ֹ���������ⷢ�� 
			// �ٽ��������  ԭ�Ӳ��� �̻߳���mutex
			// ������ڻ�ʣһ��Ʊ���߳�1����ѭ�����߳�1�õ���
			// ��ô�����߳�1��δִ�е�--��������ʱ���߳�2���뵽ѭ����������λ������
			// ��ô���߳�1ִ����Ʊʣ��Ϊ0�����ʱ���߳�2�õ�����ִ�У�Ʊʣ��Ϊ-1��
			// �������ͻ����
			// ����һ���ڼ�����������ٴμ���һ���жϣ��ٴ��ж��ٽ���Դ�Ƿ�����Ҫ����
			if (Ticketcount > 0) {
				cout << "���ڣ�" << index << "�����ڣ�" << Ticketcount << "��Ʊ" << endl;
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