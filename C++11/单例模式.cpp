#include<iostream>
#include<mutex>
using namespace std;

/*
����ģʽ��һ���಻�ܴ������ٴζ�����Զֻ�ܵõ������͵�һ�������ʵ��
A *p1 = new A();
A *p2 = new A();
A *p3 = new A();

���õ��ģ�������־ģ�飬�ܶ�ģ�齫��Ϣ����־��ģ�������һ����־���������
		  �������ݿ�ģ��

����ģʽ��
	����ʽ����ģʽ����û�л�ȡʵ������ʵ��������Ѿ�������
	����ʽ����ģʽ��Ψһ��ʵ������ֱ����һ��ʹ��֮��Ų�������
*/
#if 0
//����ʽ���� �����̰߳�ȫ��
class Singleton {
public:
	static Singleton* getInstance() {//#3 ��ȡ���Ψһʵ������Ľӿڷ���
		//������̬�ֲ������ĳ�ʼ�����ڻ��ָ�����Ѿ��Զ�����̻߳���ָ����
		static Singleton instance; //#2 ����һ��Ψһ�����ʵ������
		return &instance;
	}
private:
	Singleton() {// #1 ���캯��˽�л�
		// �ܶ��ʼ������
	}
	Singleton(const Singleton&) = delete;
	Singleton& operator=(const Singleton&) = delete;
	//Ϊ�˱�������û���������д����������ﲻ����
};

int main() {
	Singleton* p1 = Singleton::getInstance();
	Singleton* p2 = Singleton::getInstance();
	Singleton* p3 = Singleton::getInstance();

	//���������������͸�ֵ���������delete�������ǻ���������ⴴ������Ŀ���
	//���Խ���������͸�ֵ���������delete��
	//Singleton t = *p1;
	return 0;
}
#endif

#if 0
std::mutex mtx;
//����ʽ���� �����̰߳�ȫ��
class Singleton {
public:
	// �ǲ��ǿ����뺯����
	static Singleton* getInstance() {//#3 ��ȡ���Ψһʵ������Ľӿڷ���
		//lock_guard<std::mutex> guard(mtx); //��������̫����
		if (instance == nullptr) {
			lock_guard<std::mutex> guard(mtx);
			if (instance == nullptr) {
			/*
				�����ڴ�
				�������
				��instance��ֵ
			*/
				instance = new Singleton();
			}
		}
		return instance;
	}
private:
	static Singleton *volatile instance; // #2 ����һ��Ψһ�����ʵ������
	Singleton() {// #1 ���캯��˽�л�

	}
	Singleton(const Singleton&) = delete;
	Singleton& operator=(const Singleton&) = delete;
	//Ϊ�˱�������û���������д����������ﲻ����
};
//���ھ�̬��Ա��������Ҫ��������ж���
Singleton* Singleton::instance = nullptr;

int main() {
	Singleton* p1 = Singleton::getInstance();
	Singleton* p2 = Singleton::getInstance();
	Singleton* p3 = Singleton::getInstance();

	//���������������͸�ֵ���������delete�������ǻ���������ⴴ������Ŀ���
	//���Խ���������͸�ֵ���������delete��
	//Singleton t = *p1;
	return 0;
}
#endif


#if 0
//����ʽ���� һ�����̰߳�ȫ��
class Singleton {
public:
	static Singleton* getInstance() {//#3 ��ȡ���Ψһʵ������Ľӿڷ���
		return &instance;
	}
private:
	static Singleton instance; // #2 ����һ��Ψһ�����ʵ������
	Singleton() {// #1 ���캯��˽�л�

	}
	Singleton(const Singleton&) = delete;
	Singleton& operator=(const Singleton&) = delete;
	//Ϊ�˱�������û���������д����������ﲻ����
};
//���ھ�̬��Ա��������Ҫ��������ж���
Singleton Singleton::instance;

int main() {
	Singleton* p1 = Singleton::getInstance();
	Singleton* p2 = Singleton::getInstance();
	Singleton* p3 = Singleton::getInstance();

	//���������������͸�ֵ���������delete�������ǻ���������ⴴ������Ŀ���
	//���Խ���������͸�ֵ���������delete��
	//Singleton t = *p1;
	return 0;
}
#endif