#define _CRT_SECURE_NO_WARNINGS
#include<iostream>
#include<memory>
#include<thread>
#include<functional>
using namespace std;

/*
����ָ���ɾ���� deletor
����ָ�룺�ܹ���֤��Դ���Ե��ͷ� delete ptr
*/
//unique_ptr  shared_ptr
/*
~unique_ptr()
{
	//��һ����������ĵ���
	deletor(ptr);
}
template<typename T>
class Deletor
{
public:
	void operator()(T *ptr)
	{
		delete ptr;
	}
};
*/
template<typename T>
class MyDeletor {
public:
	void operator()(T* ptr)const
	{
		cout << "call MyDeletor.operator()" << endl;
		delete[] ptr;
	}
};
template<typename T>
class MyFileDeletor {
public:
	void operator()(T* ptr)const
	{
		cout << "call MyFileDeletor.operator()" << endl;
		fclose(ptr);
	}
};
int main() {
	//����Ĭ�����ͷ���Դ��ʱ����õ���delete ptr1��
	//����������Ҫ����delete[]ptr1�������Լ�дɾ����
	//����unique_ptr
	unique_ptr<int,MyDeletor<int>> ptr1(new int[100]);
	unique_ptr<FILE,MyFileDeletor<FILE>> ptr2(fopen("data.txt", "w"));
	
	//Ϊ�˽������ÿһ����Ҫ����һ��ɾ��������������ǿ�����lambda���ʽ
	//lambda���ʽ =�� �������� function
	unique_ptr<int, function<void(int*)>> ptr3(new int[100],
		[](int* p)->void {
			cout << "ͨ��lamdba���ʽ�ͷ�new int[100]" << endl;
			delete[]p;
		}
	);
	unique_ptr<FILE, function<void(FILE*)>> ptr4(fopen("data.txt", "w"),
		[](FILE* p)->void {
			cout << "ͨ��lamdba���ʽ�ͷ�fopen" << endl;
			fclose(p);
		}
	);
	return 0;
}


#if 0
/*
C++�ǳ������Ŀ�Դ����� muduo��
���̷߳��ʹ��������̰߳�ȫ����
*/
class A
{
public:
	A() { cout << "A()" << endl; }
	~A() { cout << "~A()" << endl; }
	void testA() {
		cout << "best!!!" << endl;
	}
};

//���߳�
void handler01(weak_ptr<A> pw) {
	std::this_thread::sleep_for(std::chrono::seconds(2));
	// q����A�����ʱ����Ҫ���A�����Ƿ���������̲߳���ȫ��
	shared_ptr<A> sp = pw.lock();
	if (sp != nullptr) {
		sp->testA();
	}
	else {
		cout << "A�����Ѿ������������ٴη���" << endl;
	}
}

//main�߳�
int main() {
	{
		shared_ptr<A> p(new A());
		thread t1(handler01, weak_ptr<A>(p));
		t1.detach();
	}
	std::this_thread::sleep_for(std::chrono::seconds(20));
	//���ڽ���������ָ�����ͷ���Դ������Ҫ�Լ��ͷ�
	//delete p;
	//�����ȴ����߳̽���
	//t1.join();
	return 0;
}
#endif

#if 0
/*
shared_ptr:ǿ����ָ�� ���Ըı���Դ�����ü���
weak_ptr:������ָ�� ����ı���Դ�����ü���
weak_ptr => shared_ptr => ��Դ���ڴ棩

ǿ����ָ���ѭ�����ã��������ã���ʲô���⣿ʲô�������ô�����
���new��������Դ�޷��ͷţ�����Դй¶���⣡
��������ʱ����ǿ����ָ�룻���ö���ĵط���ʹ��������ָ�룻
*/
class B;
class A 
{
public:
	A() { cout << "A()" << endl; }
	~A() { cout << "~A()" << endl; }
	void testA() {
		cout << "best!!!" << endl;
	}
	//shared_ptr<B> _ptrB;
	weak_ptr<B> _ptrB;
};
class B {
public:
	B() { cout << "B()" << endl; }
	~B() { cout << "~B()" << endl; }
	//������������ǲ��ܵ��óɹ���
	//��Ϊ������ָ��ֻ�ǹ۲���Դ�Ƿ񻹻��ţ�������ʹ�ö����еĳ�Ա����
	//��Ϊ��û��ʵ��operator* �� operator->���������
	/*void func() {
		_ptrA->testA();
	}*/
	//�����ʵ�ֵ���A�з�������Ҫ������ָ���������
	void func() {
		//_ptrA->testA();
		shared_ptr<A> ps = _ptrA.lock();//��������
		if (ps != nullptr) {
			ps->testA();
		}
	}
	//shared_ptr<A> _ptrA;
	weak_ptr<A> _ptrA;
};

int main() {
	shared_ptr<A> pa(new A());
	shared_ptr<B> pb(new B());

	pa->_ptrB = pb;
	pb->_ptrA = pa;

	cout << pa.use_count() << endl;
	cout << pb.use_count() << endl;
	pb->func();

	return 0;
}
#endif

#if 0
	//����Դ�������ü�������
	template<typename T>
	class RefCnt
	{
	public:
		RefCnt(T* ptr = nullptr)
			:mptr(ptr)
		{
			if (ptr != nullptr) {
				mcount = 1;
			}
		}
		void addRef() {
			mcount++;
		}
		int delRef() {
			return --mcount;
		}
	private:
		T* mptr;
		int mcount; //atomic_int CAS
	};

	// ����ָ�� ��֤��������Դ���Զ��ͷ�
	// ����ջ�ϵĶ�����������Զ��ͷ���Դ���ص㣬��������Դ���Զ��ͷ�
	// ���ܽ�ֻ��ָ�봴���ڶ��ϣ���Ϊ������������ָ�뻹�ǵ��Լ��ͷ�
	template<typename T>
	class CSmartPtr { //shared_ptr���˿����ڶ��߳�ģʽ��ʹ�ã��������������ָ����
	public:			  //weak_ptrҲ���̰߳�ȫ��
		CSmartPtr(T* ptr = nullptr)
			:mptr(ptr)
		{
			mpRefCnt = new RefCnt<T>(mptr);
		}
		CSmartPtr(const CSmartPtr<T>& src)
			:mptr(src.mptr), mpRefCnt(src.mpRefCnt)
		{
			if (mptr != nullptr) {
				mpRefCnt->addRef();
			}
		}
		CSmartPtr<T>& operator=(const CSmartPtr<T>& src)
		{
			if (this == &src) {
				return *this;
			}
			if (0 == mpRefCnt->delRef()) {
				delete mptr;
			}
			//ֻ�����һ��ʹ�ø���Դ��ֻ��ָ������ͷ�֮ǰ����Դ
			//����������һ��ʹ���ߣ���ôaddRef
			mptr = src.mptr;
			mpRefCnt = src.mpRefCnt;
			mpRefCnt->addRef();
			return *this;
		}
		~CSmartPtr()
		{
			if (0 == mpRefCnt->delRef())
			{
				delete mptr;
				mptr = nullptr;
			}
		}
		T& operator* () {
			return *mptr;
		}
		T* operator->() {
			return mptr;
		}

	private:
		T* mptr;//ָ����Դ��ָ��
		RefCnt<T>* mpRefCnt; //ָ����Դ���ü�����ָ��
	};
	int main() {

		/*
		�����ü���������ָ��shared_ptr��weak_ptr
		�����ü������������ָ����Թ���ͬһ����Դ
		ԭ����ÿһ��������Դ��ƥ��һ�����ü�����
			������ָ��ʹ�ø���Դ��ʱ�����ü���+1
			������ָ�벻ʹ�ø���Դ��ʱ�����ü���-1��
			���ü���!=0��ʱ�����ִ�У�Ϊ0��ʱ��������������ͷ���Դ
		*/
		CSmartPtr<int> p1(new int);
		CSmartPtr<int> p2(p1);
		CSmartPtr<int> p3;
		p3 = p2;
		*p1 = 20;
		cout << "p2��" << *p2 << "p3��" << *p3 << endl;


		/*
		�������ü���������ָ��
		auto_ptr��C++�����棬���Ƽ�ʹ��
		c++11�±�׼��
		scoped_ptr��ֱ�ӽ���������͸�ֵ���������delete����
					ֻ��ʹ�õ�������ָ�룬����������߸�ֵ��ᱨ�����Ƽ�ʹ��
		scoped_ptr(const scoped_ptr<T>&) = delete;
		scoped_ptr<T>& operator=(const scoped_ptr<T>&) = delete;
		unique_ptr���Ƽ�ʹ��
		unique_ptr(const unique_ptr<T>&) = delete;
		unique_ptr<T>& operator=(const unique_ptr<T>&) = delete;
		���ƺ�scope_ptr�ײ�һ��ԭ���������ṩ�˴���ֵ���õĿ������캯��
		unique_ptr(unique_ptr<T> &&src)
		unique_ptr<T>& operator=(unique_ptr<T> &&src)

		unique_ptr<T> getSmartPtr(){
			unique_ptr<T> ptr(new T());
			return ptr;
		}
		unique_ptr<int> ptr1 = getSmartPtr<int>();
		ptr1 = getSmartPtr<int>();

		��ô���ǳ�������⣺

		std::move =>> C++11�������� std::move�õ���ǰ��������ֵ����
		*/
		//�����û������Լ�֪�����Ѿ���p1ָ�����Դת����p2�ˣ�������������ָ���Ƽ�
		unique_ptr<int> p1(new int);
		unique_ptr<int> p2(std::move(p1));


		auto_ptr<int> p1(new int);
		auto_ptr<int> p2(p1);
		*p2 = 20;
		cout << *p1 << endl;
		//��ײ㽫֮ǰ������ָ�붼��Ϊ�գ����佻�����µ�����ָ����й���
		//����һ�㲻����ʹ�ã��ر�����ʹ��������ʱ��
		//vector<auto_ptr(new int)> vec1; vec2(vec1)
		//���ʱ��vec1�������еĵײ���ָ�붼��Ϊnullptr


		CSmartPtr<int> smartPtr(new int);
		class Test {
		public:
			void test() { cout << "call Test::test" << endl; }

		};
		CSmartPtr<Test> ptr2(new Test());
		// (ptr2.operator->())->test()
		ptr2->test();
		return 0;

	}


#endif