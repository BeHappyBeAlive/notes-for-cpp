#include<iostream>
#include<thread>
using namespace std;

#if 0
// thread�Ĵ�����һЩ��������
/*
C++���Լ���Ķ��̱߳�� =�� ������Կ�ƽ̨ʹ��
thread/mutex/condition_variable
lock_guard/unique_lock
atomic ԭ������ ����CAS������ԭ������ ���̰߳�ȫ��
˯��sleep_for

C++���Բ��� thread
	windows				linux
		|				  |
	cretateThread	 pthread_create
*/
/*
�߳����ݣ�
1. ��ô��������һ���߳�
std::thread����һ���̶߳��󣬴����߳�����Ҫ���̺߳����Ͳ�����
�߳��Զ�����ִ��
2. ���߳���ν���
���̺߳�����������߳̾ͽ�����

3. ���߳���δ������߳�
t1.join :  �ȴ�t1�߳̽�������ǰ�̼߳�����������
detach : ��t1�߳�����Ϊ�����̣߳����߳̽������������̽������������̶߳��Զ������ˣ�
		�������ػ��̣߳���ǰ���̽������ػ��߳�ȫ������
*/
void threadHandle1(int time)
{
	//�����߳�˯������
	std::this_thread::sleep_for(std::chrono::seconds(time));
	cout << "call threadHandle1()" << endl;
}
int main() {
	//������һ���̶߳��� ������һ���̺߳��� ���߳̾Ϳ�ʼ����
	std::thread t1(threadHandle1, 2);
	// ���̵߳ȴ����߳̽��������̲߳ż�����������
	//t1.join();
	// ���̱߳��������������׳��쳣�������Ҫ�����������
	// ���Խ����߳�����Ϊ�����߳�
	t1.detach();
	cout << "main thread done!" << endl;
	return 0;
}
#endif