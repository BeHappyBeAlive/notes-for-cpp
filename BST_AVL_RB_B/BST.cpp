#include<iostream>
#include<functional>
#include<stack>
#include<queue>
#include<vector>
using namespace std;
//


template<typename T, typename Compare = less<T>>
class BSTree
{
public:
	BSTree() :root_(nullptr) {}
	~BSTree()
	{

	}
	//�ǵݹ�������
	void non_insert(const T& val)
	{
		//��Ϊ�գ����ɸ��ڵ�
		if (root_ == nullptr) {
			root_ = new TreeNode(val);
			return;
		}
		//�������ʵĲ���λ�ã���¼���ڵ��λ��
		TreeNode* parent = nullptr;
		TreeNode* cur = root_;
		while (cur != nullptr) {
			if (cur->data_ == val) {
				break;
			}
			else if (compare_(cur->data_, val)) {
				parent = cur;
				cur = cur->rightchild_;
			}
			else {
				parent = cur;
				cur = cur->leftchild_;
			}
		}
		if (val < parent->data_) {
			parent->leftchild_ = new TreeNode(val);
		}
		else {
			parent->rightchild_ = new TreeNode(val);
		}


	}
	//�ݹ�������
	void insert(const T& val) {
		//�ݹ���Ҫ���ݣ�������Ҫ����һ���ڵ㽫����������
		//����ǰ�ڵ���丸�ڵ㷵��
		root_ = insert(root_, val);
	}
	//�ǵݹ�ɾ������
	void non_remove(const T& val) {
		if (root_ == nullptr) {
			return;
		}
		TreeNode* parent = nullptr;
		TreeNode* cur = root_;
		while (cur != nullptr) {
			if (cur->data_ == val) {
				break;
			}
			else if (compare_(cur->data_, val)) {
				parent = cur;
				cur = cur->rightchild_;
			}
			else {
				parent = cur;
				cur = cur->leftchild_;
			}
		}

		//δ�ҵ���ɾ���ڵ�
		if (cur == nullptr) {
			return;
		}

		//�ҵ���ɾ���ڵ�
		//���3 ��ɾ���ڵ�����������
		if (cur->leftchild_ != nullptr && cur->rightchild_ != nullptr) {
			TreeNode* pre = cur->leftchild_;
			//����ֱ��ǰ�� ����������������Ǹ��ڵ�
			while (pre->rightchild_ != nullptr) {
				parent = pre;
				pre = pre->rightchild_;
			}
			cur->data_ = pre->data_;
			//��curָ��ǰ���ڵ㣬ת�������1��2
			cur = pre;
		}
		//curָ��ɾ���ڵ㣬parentָ���丸�ڵ㣬
		//ͳһ����curָ��Ľڵ㣬���1��2
		TreeNode* child = cur->leftchild_;
		if (child == nullptr) {
			child = cur->rightchild_;
		}
		if (parent == nullptr) { //˵��ɾ�����Ǹ��ڵ�
			root_ = child;
		}
		else 
		{
			//�Ѵ�ɾ���ڵ�ĺ���(nullptr���߲�Ϊ��)д�뵽�丸�ڵ���Ӧ�ĵ�ַ����
			if (parent->leftchild_ == cur) {
				parent->leftchild_ = child;
			}
			else {
				parent->rightchild_ = child;
			}
		}
		
		delete cur;//ɾ����ǰ�ڵ�
	}
	//�ݹ�ɾ������
	void remove(const T& val) {
		remove(root_, val);
	}
	//�ǵݹ�Ĳ�ѯ����
	bool non_query(const T& val) {
		if (root_ == nullptr) {
			return false;
		}
		TreeNode* cur = root_;
		while (cur != nullptr) {
			if (cur->data_ == val) {
				//�ҵ���
				return true;
			}
			else if (compare_(cur->data_, val)) {
				cur = cur->rightchild_;
			}
			else
			{
				cur = cur->leftchild_;
			}
		}
		//û���ҵ�
		return false;
	}
	//�ݹ�Ĳ�ѯ����
	bool query(const T& val) {
		return nullptr != query(root_, val);
	}
	//�ݹ�ǰ�����
	void preOrder() {
		cout << "�ݹ�ǰ�������"  ;
		preOrder(root_);
		cout << endl;
	}
	//�ǵݹ�ǰ���������
	void non_preOrder() {
		cout << "�ǵݹ�ǰ�������";
		if (root_ == nullptr) {
			return;
		}
		stack<TreeNode*> stk;
		stk.push(root_);
		while (!stk.empty()) {
			TreeNode* top = stk.top();
			stk.pop();
			cout << top->data_ << " ";//V

			if (top->rightchild_ != nullptr) {
				stk.push(top->rightchild_);//R
			}
			if (top->leftchild_ != nullptr) {
				stk.push(top->leftchild_);//L
			}
		}
		cout << endl;
	}
	//�ݹ��������
	void inOrder() {
		cout << "�ݹ����������" ;
		inOrder(root_);
		cout << endl;
	}
	//�ǵݹ��������
	void non_inOrder() {
		cout << "�ǵݹ����������";
		if (root_ == nullptr) {
			return;
		}
		stack<TreeNode*> stk;
		TreeNode* cur = root_;
		while (!stk.empty()||cur!=nullptr) {
			if (cur != nullptr) {
				stk.push(cur);
				cur = cur->leftchild_;
			}
			else {
				TreeNode* top = stk.top();
				stk.pop();
				cout << top->data_ << " ";
				cur = top->rightchild_;
			}
		}
		cout << endl;
	}
	//�ݹ�������
	void postOrder() {
		cout << "�ݹ���������"  ;
		postOrder(root_);
		cout << endl;
	}
	//�ǵݹ������� LRV => VRL ������ջʵ�� 
	void non_postOrder() {
		cout << "�ǵݹ���������";
		if (root_ == nullptr) {
			return;
		}
		stack<TreeNode*> stk1;
		stack<TreeNode*> stk2;
		stk1.push(root_);
		while (!stk1.empty()) {
			//ջ1��ջ
			TreeNode* top = stk1.top();
			stk1.pop();

			//ջ2��ջ
			stk2.push(top);//V
			if (top->leftchild_ != nullptr) {//L
				stk1.push(top->leftchild_);
			}
			if (top->rightchild_ != nullptr) {//R
				stk1.push(top->rightchild_);
			}
		}
		while (!stk2.empty()) {
			TreeNode* top = stk2.top();
			stk2.pop();
			cout << top->data_ << " ";
		}
		cout << endl;
	}
	//�ݹ�����������
	void levelOrder() {
		cout << "�ݹ�������:";
		int h = high(); //���Ĳ���
		for (int i = 0; i < h; ++i) {
			levelOrder(root_, i); //�ݹ���������ʵ��
		}
		cout << endl;
	}
	//�ǵݹ�����������
	void non_levelOrder() {
		cout << "�ǵݹ���������";
		if (root_ == nullptr) {
			return;
		}
		queue<TreeNode*> que;
		que.push(root_);
		while (!que.empty()) {
			TreeNode* front = que.front();
			que.pop();
			cout << front->data_ << " ";
			if (front->leftchild_ != nullptr) {
				que.push(front->leftchild_);
			}
			if (front->rightchild_ != nullptr) {
				que.push(front->rightchild_);
			}
		}
		cout << endl;
	}
	//�ݹ��ȡ����
	int high() {
		return level(root_);
	}
	//�ݹ���������ڵ����
	int number() {
		return number(root_);
	}

	//�����������Ԫ��ֵ[i,j]
	void findValues(vector<T>& vec, int i, int j) {
		findValues(root_, vec, i, j);
	}

	//�ж϶������Ƿ���BST��
	bool isBSTree()
	{
		TreeNode* pre = nullptr;
		return isBSTree(root_, pre);
	}

	//�ж���������
	bool isChildTree(BSTree<T, Compare>& child) {
		//�ڵ�ǰ����������child�ĸ��ڵ�
		if (child.root_ == nullptr) {
			return true;
		}

		TreeNode* cur = root_;
		while (cur != nullptr) {
			if (cur->data_ == child.root_->data_) {
				//�ҵ��˸��ڵ�
				break;
			}
			else if (compare_(cur->data_, child.root_->data_)) {
				cur = cur->rightchild_;
			}
			else {
				cur = cur->leftchild_;
			}
		}
		if (cur == nullptr) {
			return false;
		}
		//Ȼ��Ƚϴӵ�ǰ�ڵ㿪ʼ��
		//���ṩ�������ĸ��ڵ���бȽϣ��Ƚ����е�ÿһ��Ԫ��
		return isChildTree(cur, child.root_);

	}

private:
	//�ڵ㶨��
	struct TreeNode
	{
		TreeNode(T data = T())
			:data_(data), leftchild_(nullptr), rightchild_(nullptr)
		{

		}
		T data_; //������
		TreeNode* leftchild_; //������
		TreeNode* rightchild_; //�Һ�����

	};
	//ǰ����� VLR
	void preOrder(TreeNode* root) {
		if (root != nullptr) {
			cout << root->data_ << " ";
			preOrder(root->leftchild_);
			preOrder(root->rightchild_);
		}
	}
	//������� LVR
	void inOrder(TreeNode* root) {
		if (root != nullptr) {
			inOrder(root->leftchild_);
			cout << root->data_ << " ";
			inOrder(root->rightchild_);
		}
	}
	//������� LRV
	void postOrder(TreeNode* root) {
		if (root != nullptr) {
			postOrder(root->leftchild_);
			postOrder(root->rightchild_);
			cout << root->data_ << " ";
		}
	}
	//�ݹ���BST�Ĳ��� ����nodeΪ���ڵ�������ĸ߶Ȳ��ҷ��ظ߶�ֵ
	int level(TreeNode* node) {
		if (node == nullptr) {
			return 0;
		}
		int left = level(node->leftchild_);
		int right = level(node->rightchild_);
		return left > right ? left + 1 : right + 1;
	}
	//�ݹ���BST�Ľڵ����� ����nodeΪ���ڵ�����Ľڵ����������ҷ���
	int number(TreeNode* node) {
		if (node == nullptr) {
			return 0;
		}
		int left = number(node->leftchild_);//L
		int right = number(node->rightchild_);//R
		return left + right + 1;//V
	}
	//�ݹ�������ʵ��
	void levelOrder(TreeNode* node, int i) {
		if (node == nullptr) {
			return;
		}
		if (i == 0) {
			cout << node->data_ << " ";
		}
		levelOrder(node->leftchild_, i - 1);
		levelOrder(node->rightchild_, i - 1);
	}
	//�ݹ�ʵ�ֲ���
	TreeNode* insert(TreeNode* node, const T& val) {
		if (node == nullptr) {
			//�ݹ�������ҵ�����val��λ�ã������½ڵ㣬������ڵ�
			return new TreeNode(val);
		}
		if (node->data_ == val) {
			return node;
		}
		else if (compare_(node->data_, val)) {
			node->rightchild_ = insert(node->rightchild_, val);
		}
		else {
			node->leftchild_ = insert(node->leftchild_, val);
		}
		return node;
	}
	//�ݹ�ʵ�ֲ�ѯ
	TreeNode* query(TreeNode* node, const T& val) {
		if (node == nullptr) {
			return nullptr;
		}
		if (node->data_ == val) {
			return node;
		}
		else if (compare_(node->data_, val)) {
			return query(node->rightchild_, val);
		}
		else {
			return query(node->leftchild_, val);
		}

		return nullptr;
	}
	//�ݹ�ʵ��ɾ��
	TreeNode* remove(TreeNode* node, const T& val) {
		if (node == nullptr) {
			//û�ҵ�
			return nullptr;
		}
		
		if (node->data_ == val) {
			//�ҵ���ɾ���ڵ�
			//���3 �����ӽڵ�
			if (node->leftchild_ != nullptr && node->rightchild_ != nullptr) {
				//�������ӽڵ�
				TreeNode* pre = node->leftchild_;
				while (pre->rightchild_ != nullptr) {
					pre = pre->rightchild_;
				}
				//�ҵ�ǰ���ڵ�Ϊpre������ǰ�ڵ㸲�ǵ�����pre��ֵ
				node->data_ = pre->data_;
				//�ݹ�ɾ��ǰ���ڵ�
				node->leftchild_ = remove(node->leftchild_, pre->data_);
			}
			else {//���1�����2
				if (node->leftchild_ != nullptr) {//��һ������
					TreeNode* left = node->leftchild_;
					delete node;
					return left;
				}
				else if (node->rightchild_ != nullptr) {//��һ���Һ���
					TreeNode* right = node->rightchild_;
					delete node;
					return right;
				}
				else { //ɾ������û�к��ӵĽڵ� Ҷ�ӽ��
					delete node;
					return nullptr;
				}
			}
		}
		else if (compare_(node->data_, val)) {
			node->rightchild_ = remove(node->rightchild_, val);
		}
		else {
			node->leftchild_ = remove(node->leftchild_, val);
		}
		return node;
	}

	//�����������Ԫ��ֵ[i,j]
	void findValues(TreeNode* node, vector<T>& vec, int i, int j) {
		if (node != nullptr) {
			//�ڵ�ǰ�ڵ��������������
			if (node->data_ > i) {
				findValues(node->leftchild_, vec, i, j);//L
			}
			//V
			if (node->data_ >= i && node->data_ <= j) {
				//�洢���������ֵ���������ｫ�����������ˣ�
				//�����Ż����Ż����Ǹ��ݱ߽������ж��Ƿ�����������̽��
				vec.push_back(node->data_);
			}
			//�ڵ�ǰ�ڵ��������������
			if (node->data_ < j) {
				findValues(node->rightchild_, vec, i, j);//R
			}
		}
	}

	//�ж϶������Ƿ���BST��
	bool isBSTree(TreeNode* node, TreeNode*& pre) {//����������Ϊ��Ҫ��¼��ǰ�ڵ����������ǰһ���ڵ��ֵ��
		if (node == nullptr) {					//���������ÿһ�εݹ���ݵĹ����У�ջ��pre�ڴ��ֵ�᲻һ��
			return true;
		}
		if (isBSTree(node->leftchild_, pre)) {//L ��Ҫ�ж��ǵݹ����������
			return false;
		}
		//V
		if (pre != nullptr)
		{
			if (compare_(node->data_, pre->data_)) { //��Ҫ�ж��ǵݹ����������
				return false;
			}
		}
		pre = node;//�������������ǰ���ڵ�

		return isBSTree(node->rightchild_, pre);//R
		
		/*
		//V
		if (node->leftchild_ != nullptr && compare_(node->data_, node->leftchild_->data_)) {
			return false;
		}
		if (node->rightchild_ != nullptr && compare_(node->rightchild_->data_, node->data_)) {
			return false;
		}
		if (!isBSTree(node->leftchild_)) {//L �жϵ�ǰ�ڵ��������
			return false;
		}
		if (!isBSTree(node->rightchild_)) {//R �жϵ�ǰ�ڵ��������
			return false;
		}
		else {
			return true;
		}*/
		//�������ַ�����Щ�������ȷ��ֻ�Ǿֲ��ж�

	}

	bool isChildTree(TreeNode* father, TreeNode* child) {
		if (father == nullptr && child == nullptr) {
			return true;
		}
		if (father == nullptr) {
			return false; //���������еĽڵ㵱ǰ�ڵ�û��
		}
		if (child == nullptr) {
			return true;	//��������Ľڵ�������ڵ�࣬����Խ���
		}
		//�ж�ֵ����ͬ
		if (father->data_ != child->data_) {//V
			return false;
		}

		return isChildTree(father->leftchild_, child->leftchild_)//L
			&& isChildTree(father->rightchild_, child->rightchild_);//R
	}



	TreeNode* root_;
	Compare compare_;
};

#if 0
void test01() {//�����Ƿ���BST��
	using Node = BSTree<int>::TreeNode;
	BSTree<int> bst;
	bst.root_ = new Node(40);
	Node* node1 = new Node(20);
	Node* node2 = new Node(60);
	Node* node3 = new Node(30);
	Node* node4 = new Node(80);

	bst.root_->leftchild_ = node1;
	bst.root_->rightchild_ = node2;
	node2->leftchild_ = node3;
	node2->rightchild_ = node4;

	cout << bst.isBSTree() << endl;
}
void test02() { //��������
	int arr[] = { 54,24,67,0,34,62,69,5,41,64,78 };
	BSTree<int> bst;
	for (int v : arr) {
		//bst.non_insert(v);
		bst.insert(v);
	}

	using Node = BSTree<int>::TreeNode;
	BSTree<int> bst1;
	bst1.root_ = new Node(67);
	Node* node1 = new Node(62);
	Node* node2 = new Node(69);
	Node* node3 = new Node(60);
	bst1.root_->leftchild_ = node1;
	bst1.root_->rightchild_ = node2;
	//node1->leftchild_ = node3;

	cout << bst.isChildTree(bst1) << endl;


}
#endif


int main() {
	//test01();
	test02();
#if 0
	int arr[] = { 54,24,67,0,34,62,69,5,41,64,78 };
	BSTree<int> bst;
	for (int v : arr) {
		//bst.non_insert(v);
		bst.insert(v);
	}
	bst.non_insert(12);
	//cout << bst.non_query(12);
	//bst.non_remove(12);
	bst.remove(12);
	//cout << bst.non_query(12);
	bst.inOrder();
	bst.non_inOrder();
	bst.preOrder();
	bst.non_preOrder();
	bst.postOrder();
	bst.non_postOrder();
	bst.levelOrder();
	bst.non_levelOrder();
	vector<int> vec;
	bst.findValues(vec, 0, 34);
	for (int v : vec) {
		cout << v << " ";
	}
	cout << endl;
#endif

	return 0;
}