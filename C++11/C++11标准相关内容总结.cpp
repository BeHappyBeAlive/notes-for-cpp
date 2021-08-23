#include<iostream>
using namespace std;

/*
C++11标准相关的内容

一. 关键字和语法
auto: 可以根据右值，推导出右值的类型，然后左边变量的类型也就已知了
nullptr: 给指针专用(能够和正数进行区别) #define NULL 0
foreach: 可以遍历数组、容器等
for(Type val : Container) => 底层就是通过指针或者迭代器实现
{
	cout<<val<<endl;
}
右值引用： move 移动语义函数和forward类型完美转发函数
模板的一个新特性： typename... A表示可变参数 （类型参数）

二. 绑定器和函数对象
function: 函数对象
bind : 绑定器 bind1st和bind2st+ 二元函数对象 => 一元函数对象
lambda表达式

三. 智能指针
shared_ptr和weak_ptr

四. 容器
set和map : 红黑树O(lgn)
unoredered_set和unordered_map:哈希表O(1)
array: 数组 vector
forward_list:前向链表 list

五. C++语言级别支持的多线程编程
createThread windows
pthread_create Linux
clone Linux
*/
