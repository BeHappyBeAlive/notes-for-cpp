#include<iostream>
#include<vector>
#include<functional>
#include<algorithm>
#include<ctime>
using namespace std;

#if 0
//自己实现的绑定器bind1st
/*
绑定器和函数对象operator()

1. C++中STL中的绑定器
bind1st：operator()的第一个行参变量绑定成一个确定的值
bind2st：operator()的第二个形参变量绑定成一个确定的值

2. C++11从Boost库中引入了bind绑定器和function函数对象机制

3. lambda表达式 底层依赖函数对象的机制实现的
*/
template<typename Container>
void showContainer(Container& con) {
	typename Container::iterator it = con.begin();
	for (; it != con.end(); ++it) {
		cout << *it << " ";
	}
	cout << endl;
}

template<typename Iterator,typename Compare>
Iterator my_find_if(Iterator first,Iterator last,Compare comp)
{ 
	for (; first != last; ++first) {
		if (comp(*first)) {
			return first;
		}
	}
	return last;

}

template<typename Compare,typename T>
class _mybind1st {
public:
	_mybind1st(Compare comp,T val)
		:_comp(comp),_val(val)
	{

	}
	bool operator() (const T& second){
		return _comp(_val, second);
	}

private:
	Compare _comp;
	T _val;

};

template<typename Compare,typename T>
_mybind1st<Compare,T> mybind1st(Compare comp, const T& val) 
{
	//直接使用函数模板好处是可以进行类型的推演

	return _mybind1st<Compare, T>(comp, val);
}

int main() {
	vector<int> vec;
	srand(time(nullptr));
	for (int i = 0; i < 20; ++i) {
		vec.push_back(rand() % 100 + 1);
	}
	showContainer(vec);
	sort(vec.begin(), vec.end());//默认由小到大
	showContainer(vec);
	//需要二元函数对象
	sort(vec.begin(), vec.end(), greater<int>());//从大到小排序
	showContainer(vec);

	//把70按照顺序插入到vec容器中 找第一个小于70的数字
	/*
	operator()(const T &val);
	greator a>b
	less a<b
	绑定器 + 二元函数对象 =》 一元函数对象
	bind1st：greator bool operator(const _Ty& _Left,const _Ty& _Right)
	绑定第一个参数为70
	bind2st：less bool operator(const _Ty& _Left,const _Ty& _Right)
	绑定第二个参数为70
	*/
	auto it1 = my_find_if(vec.begin(), vec.end(), mybind1st(greater<int>(), 70));
	if (it1 != vec.end()) {
		vec.insert(it1, 70);
	}
	showContainer(vec);
	return 0;
}
#endif


#if 0
/*
绑定器和函数对象operator()

1. C++中STL中的绑定器
bind1st：operator()的第一个行参变量绑定成一个确定的值
bind2st：operator()的第二个形参变量绑定成一个确定的值

2. C++11从Boost库中引入了bind绑定器和function函数对象机制

3. lambda表达式 底层依赖函数对象的机制实现的
*/
template<typename Container>
void showContainer(Container& con) {
	typename Container::iterator it = con.begin();
	for (; it != con.end(); ++it) {
		cout << *it << " ";
	}
	cout << endl;
}
int main() {
	vector<int> vec;
	srand(time(nullptr));
	for (int i = 0; i < 20; ++i) {
		vec.push_back(rand() % 100 + 1);
	}
	showContainer(vec);
	sort(vec.begin(), vec.end());//默认由小到大
	showContainer(vec);
	//需要二元函数对象
	sort(vec.begin(), vec.end(),greater<int>());//从大到小排序
	showContainer(vec);

	//把70按照顺序插入到vec容器中 找第一个小于70的数字
	/*
	operator()(const T &val);
	greator a>b
	less a<b
	绑定器 + 二元函数对象 =》 一元函数对象
	bind1st：greator bool operator(const _Ty& _Left,const _Ty& _Right)
	绑定第一个参数为70
	bind2st：less bool operator(const _Ty& _Left,const _Ty& _Right)
	绑定第二个参数为70
	*/
	auto it1 = find_if(vec.begin(), vec.end(), bind1st(greater<int>(), 70));
	if (it1 != vec.end()) {
		vec.insert(it1, 70);
	}
	showContainer(vec);
	return 0;
}
#endif