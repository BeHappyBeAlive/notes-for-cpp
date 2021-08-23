#include<iostream>
#include<vector>
#include<functional>
#include<algorithm>
#include<ctime>
using namespace std;

#if 0
//�Լ�ʵ�ֵİ���bind1st
/*
�����ͺ�������operator()

1. C++��STL�еİ���
bind1st��operator()�ĵ�һ���вα����󶨳�һ��ȷ����ֵ
bind2st��operator()�ĵڶ����βα����󶨳�һ��ȷ����ֵ

2. C++11��Boost����������bind������function�����������

3. lambda���ʽ �ײ�������������Ļ���ʵ�ֵ�
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
	//ֱ��ʹ�ú���ģ��ô��ǿ��Խ������͵�����

	return _mybind1st<Compare, T>(comp, val);
}

int main() {
	vector<int> vec;
	srand(time(nullptr));
	for (int i = 0; i < 20; ++i) {
		vec.push_back(rand() % 100 + 1);
	}
	showContainer(vec);
	sort(vec.begin(), vec.end());//Ĭ����С����
	showContainer(vec);
	//��Ҫ��Ԫ��������
	sort(vec.begin(), vec.end(), greater<int>());//�Ӵ�С����
	showContainer(vec);

	//��70����˳����뵽vec������ �ҵ�һ��С��70������
	/*
	operator()(const T &val);
	greator a>b
	less a<b
	���� + ��Ԫ�������� =�� һԪ��������
	bind1st��greator bool operator(const _Ty& _Left,const _Ty& _Right)
	�󶨵�һ������Ϊ70
	bind2st��less bool operator(const _Ty& _Left,const _Ty& _Right)
	�󶨵ڶ�������Ϊ70
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
�����ͺ�������operator()

1. C++��STL�еİ���
bind1st��operator()�ĵ�һ���вα����󶨳�һ��ȷ����ֵ
bind2st��operator()�ĵڶ����βα����󶨳�һ��ȷ����ֵ

2. C++11��Boost����������bind������function�����������

3. lambda���ʽ �ײ�������������Ļ���ʵ�ֵ�
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
	sort(vec.begin(), vec.end());//Ĭ����С����
	showContainer(vec);
	//��Ҫ��Ԫ��������
	sort(vec.begin(), vec.end(),greater<int>());//�Ӵ�С����
	showContainer(vec);

	//��70����˳����뵽vec������ �ҵ�һ��С��70������
	/*
	operator()(const T &val);
	greator a>b
	less a<b
	���� + ��Ԫ�������� =�� һԪ��������
	bind1st��greator bool operator(const _Ty& _Left,const _Ty& _Right)
	�󶨵�һ������Ϊ70
	bind2st��less bool operator(const _Ty& _Left,const _Ty& _Right)
	�󶨵ڶ�������Ϊ70
	*/
	auto it1 = find_if(vec.begin(), vec.end(), bind1st(greater<int>(), 70));
	if (it1 != vec.end()) {
		vec.insert(it1, 70);
	}
	showContainer(vec);
	return 0;
}
#endif