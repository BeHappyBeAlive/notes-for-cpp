#include<iostream>
#include<thread>
#include<mutex>
#include<condition_variable> // ��������
#include<queue> // C++ STL���е������������̰߳�ȫ��
using namespace std;
#if 0
/*
unique_lock��Ҫ���condition_variableʹ��
1. lock_guard��unique_lock
2. condition_variable  wait��notify_all����
*/
std::mutex mtx;
std::condition_variable cv;
int main() {

	/*
	֪ͨ��cv�ϵȴ����̣߳��������������������ɻ���
	����cv�ϵȴ����̣߳��յ�֪ͨ���ӵȴ�״̬=������״̬=���õ�������=���̼߳�������
	*/
	cv.notify_all();



	//�������������ڼ򵥵��ٽ�������εĻ�������У������ٺ������ù�����
	unique_lock<std::mutex> lck(mtx);
	//#1 ʹ�߳̽���ȴ�״̬ #2 ��������mtx�ͷŵ�����Ϊ���е�����lck.unlock()
	cv.wait(lck);
	//���������������ʧ
	//��Ϊ��������͸�ֵ���ض�delete�ˣ����Բ������ں����������ݻ��߷��ع����У�
	//ֻ�����ڼ򵥵��ٽ�������εĻ�������У�����������unique_lock
	//lock_guard<std::mutex> guard(mtx);

	//�򵥵Ļ�����
	//mtx.lock();

	//mtx.unlock();

	return 0;
}
#endif
