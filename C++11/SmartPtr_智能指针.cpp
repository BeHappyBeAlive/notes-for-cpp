#define _CRT_SECURE_NO_WARNINGS
#include<iostream>
#include<memory>
#include<thread>
#include<functional>
using namespace std;
#if 0
/*
智能指针的删除器 deletor
智能指针：能够保证资源绝对的释放 delete ptr
*/
//unique_ptr  shared_ptr
/*
~unique_ptr()
{
	//是一个函数对象的调用
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
	//下面默认在释放资源的时候调用的是delete ptr1，
	//而这里面需要的是delete[]ptr1，所以自己写删除器
	//传给unique_ptr
	unique_ptr<int,MyDeletor<int>> ptr1(new int[100]);
	unique_ptr<FILE,MyFileDeletor<FILE>> ptr2(fopen("data.txt", "w"));
	
	//为了解决上面每一个都要定义一个删除器的情况，我们可以用lambda表达式
	//lambda表达式 =》 函数对象 function
	unique_ptr<int, function<void(int*)>> ptr3(new int[100],
		[](int* p)->void {
			cout << "通过lamdba表达式释放new int[100]" << endl;
			delete[]p;
		}
	);
	unique_ptr<FILE, function<void(FILE*)>> ptr4(fopen("data.txt", "w"),
		[](FILE* p)->void {
			cout << "通过lamdba表达式释放fopen" << endl;
			fclose(p);
		}
	);
	return 0;
}
#endif

#if 0
/*
C++非常著名的开源网络库 muduo库
多线程访问共享对象的线程安全问题
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

//子线程
void handler01(weak_ptr<A> pw) {
	std::this_thread::sleep_for(std::chrono::seconds(2));
	// q访问A对象的时候，需要侦测A对象是否存活，否则是线程不安全的
	shared_ptr<A> sp = pw.lock();
	if (sp != nullptr) {
		sp->testA();
	}
	else {
		cout << "A对象已经析构，不能再次访问" << endl;
	}
}

//main线程
int main() {
	{
		shared_ptr<A> p(new A());
		thread t1(handler01, weak_ptr<A>(p));
		t1.detach();
	}
	std::this_thread::sleep_for(std::chrono::seconds(20));
	//现在交给了智能指针来释放资源，不需要自己释放
	//delete p;
	//阻塞等待子线程结束
	//t1.join();
	return 0;
}
#endif

#if 0
/*
shared_ptr:强智能指针 可以改变资源的引用计数
weak_ptr:弱智能指针 不会改变资源的引用计数
weak_ptr => shared_ptr => 资源（内存）

强智能指针的循环引用（交叉引用）是什么问题？什么结果？怎么解决？
造成new出来的资源无法释放！！资源泄露问题！
定义对象的时候用强智能指针；引用对象的地方，使用弱智能指针；
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
	//下面这个函数是不能调用成功的
	//因为弱智能指针只是观察资源是否还活着，并不能使用对象中的成员方法
	//因为其没有实现operator* 和 operator->运算符重载
	/*void func() {
		_ptrA->testA();
	}*/
	//该如何实现调用A中方法，需要对智能指针进行提升
	void func() {
		//_ptrA->testA();
		shared_ptr<A> ps = _ptrA.lock();//提升方法
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
	//对资源进行引用计数的类
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

	// 智能指针 保证能做到资源的自动释放
	// 利用栈上的对象出作用域自动释放资源的特点，来做到资源的自动释放
	// 不能将只能指针创建在堆上，因为这样对于智能指针还是得自己释放
	template<typename T>
	class CSmartPtr { //shared_ptr除了可以在多线程模式下使用，其余与这个智能指针差不多
	public:			  //weak_ptr也是线程安全的
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
			//只有最后一个使用该资源的只能指针才能释放之前的资源
			//如果不是最后一个使用者，那么addRef
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
		T* mptr;//指向资源的指针
		RefCnt<T>* mpRefCnt; //指向资源引用计数的指针
	};
	int main() {

		/*
		带引用计数的智能指针shared_ptr和weak_ptr
		带引用计数：多个智能指针可以管理同一个资源
		原理：给每一个对象资源，匹配一个引用计数，
			当智能指针使用该资源的时候，引用计数+1
			当智能指针不使用该资源的时候，引用计数-1，
			引用计数!=0的时候继续执行，为0的时候调用析构函数释放资源
		*/
		CSmartPtr<int> p1(new int);
		CSmartPtr<int> p2(p1);
		CSmartPtr<int> p3;
		p3 = p2;
		*p1 = 20;
		cout << "p2：" << *p2 << "p3：" << *p3 << endl;


		/*
		不带引用计数的智能指针
		auto_ptr：C++库里面，不推荐使用
		c++11新标准：
		scoped_ptr：直接将拷贝构造和赋值运算符重载delete掉，
					只能使用单个智能指针，如果拷贝或者赋值则会报错，不推荐使用
		scoped_ptr(const scoped_ptr<T>&) = delete;
		scoped_ptr<T>& operator=(const scoped_ptr<T>&) = delete;
		unique_ptr：推荐使用
		unique_ptr(const unique_ptr<T>&) = delete;
		unique_ptr<T>& operator=(const unique_ptr<T>&) = delete;
		看似和scope_ptr底层一样原理，但是其提供了带右值引用的拷贝构造函数
		unique_ptr(unique_ptr<T> &&src)
		unique_ptr<T>& operator=(unique_ptr<T> &&src)

		unique_ptr<T> getSmartPtr(){
			unique_ptr<T> ptr(new T());
			return ptr;
		}
		unique_ptr<int> ptr1 = getSmartPtr<int>();
		ptr1 = getSmartPtr<int>();

		怎么解决浅拷贝问题：

		std::move =>> C++11优质引用 std::move得到当前变量的右值类型
		*/
		//这里用户可以自己知道，已经将p1指针的资源转给了p2了，所以这种智能指针推荐
		unique_ptr<int> p1(new int);
		unique_ptr<int> p2(std::move(p1));


		auto_ptr<int> p1(new int);
		auto_ptr<int> p2(p1);
		*p2 = 20;
		cout << *p1 << endl;
		//其底层将之前的智能指针都置为空，将其交给最新的智能指针进行管理
		//所以一般不建议使用，特别是在使用容器的时候
		//vector<auto_ptr(new int)> vec1; vec2(vec1)
		//这个时候vec1里面所有的底层裸指针都置为nullptr


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
