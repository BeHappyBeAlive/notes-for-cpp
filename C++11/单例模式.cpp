#include<iostream>
#include<mutex>
using namespace std;

/*
单例模式：一个类不管创建多少次对象，永远只能得到该类型的一个对象的实例
A *p1 = new A();
A *p2 = new A();
A *p3 = new A();

常用到的，比如日志模块，很多模块将信息给日志的模块对象，由一个日志对象来解决
		  比如数据库模块

单例模式：
	饿汉式单例模式：还没有获取实例对象，实例对象就已经产生了
	懒汉式单例模式：唯一的实例对象，直到第一次使用之后才产生对象
*/
#if 0
//懒汉式单例 不是线程安全的
class Singleton {
public:
	static Singleton* getInstance() {//#3 获取类的唯一实例对象的接口方法
		//函数静态局部变量的初始化，在汇编指令上已经自动添加线程互斥指令了
		static Singleton instance; //#2 定义一个唯一的类的实例对象
		return &instance;
	}
private:
	Singleton() {// #1 构造函数私有化
		// 很多初始化代码
	}
	Singleton(const Singleton&) = delete;
	Singleton& operator=(const Singleton&) = delete;
	//为了避免出现用户在外面进行创建对象，这里不仅将
};

int main() {
	Singleton* p1 = Singleton::getInstance();
	Singleton* p2 = Singleton::getInstance();
	Singleton* p3 = Singleton::getInstance();

	//如果不讲拷贝构造和赋值运算符重载delete掉，则还是会出现在类外创建对象的可能
	//所以将拷贝构造和赋值运算符重载delete掉
	//Singleton t = *p1;
	return 0;
}
#endif

#if 0
std::mutex mtx;
//懒汉式单例 不是线程安全的
class Singleton {
public:
	// 是不是可重入函数？
	static Singleton* getInstance() {//#3 获取类的唯一实例对象的接口方法
		//lock_guard<std::mutex> guard(mtx); //锁的粒度太大了
		if (instance == nullptr) {
			lock_guard<std::mutex> guard(mtx);
			if (instance == nullptr) {
			/*
				开辟内存
				构造对象
				给instance赋值
			*/
				instance = new Singleton();
			}
		}
		return instance;
	}
private:
	static Singleton *volatile instance; // #2 定义一个唯一的类的实例对象
	Singleton() {// #1 构造函数私有化

	}
	Singleton(const Singleton&) = delete;
	Singleton& operator=(const Singleton&) = delete;
	//为了避免出现用户在外面进行创建对象，这里不仅将
};
//对于静态成员变量，需要在类外进行定义
Singleton* Singleton::instance = nullptr;

int main() {
	Singleton* p1 = Singleton::getInstance();
	Singleton* p2 = Singleton::getInstance();
	Singleton* p3 = Singleton::getInstance();

	//如果不讲拷贝构造和赋值运算符重载delete掉，则还是会出现在类外创建对象的可能
	//所以将拷贝构造和赋值运算符重载delete掉
	//Singleton t = *p1;
	return 0;
}
#endif


#if 0
//饿汉式单例 一定是线程安全的
class Singleton {
public:
	static Singleton* getInstance() {//#3 获取类的唯一实例对象的接口方法
		return &instance;
	}
private:
	static Singleton instance; // #2 定义一个唯一的类的实例对象
	Singleton() {// #1 构造函数私有化

	}
	Singleton(const Singleton&) = delete;
	Singleton& operator=(const Singleton&) = delete;
	//为了避免出现用户在外面进行创建对象，这里不仅将
};
//对于静态成员变量，需要在类外进行定义
Singleton Singleton::instance;

int main() {
	Singleton* p1 = Singleton::getInstance();
	Singleton* p2 = Singleton::getInstance();
	Singleton* p3 = Singleton::getInstance();

	//如果不讲拷贝构造和赋值运算符重载delete掉，则还是会出现在类外创建对象的可能
	//所以将拷贝构造和赋值运算符重载delete掉
	//Singleton t = *p1;
	return 0;
}
#endif