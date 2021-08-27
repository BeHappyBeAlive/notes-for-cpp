#include<iostream>
#include<algorithm>
using namespace std;


#if 0
template<typename T>
class AVLTree
{
public:
	AVLTree() :root_(nullptr) 
	{

	}

	//AVL���Ĳ���ʵ��
	void insert(const T& val) {
		root_ = insert(root_, val);
	}
	void remove(const T& val) {
		root_ = remove(root_, val);
	}
private:

	//�������Ľڵ�
	struct Node 
	{
		Node(T data = T())
			:data_(data),
			left_(nullptr),
			right_(nullptr),
			height(1)
		{

		}
		T data_;
		Node* left_;
		Node* right_;
		int height;
	};

	int height(Node* node)
	{
		return node == nullptr ? 0 : node->height;
	}


	//����ת���� ��node�ڵ�Ϊ��������ת�����������µĸ��ڵ㷵��
	Node* rightRotate(Node* node)
	{
		Node* child = node->left_;
		node->left_ = child->right_;
		child->right_ = node;
		//�߶ȸ���
		node->height = max(height(node->left_), height(node->right_)) + 1;
		child->height = max(height(child->left_), height(child->right_)) + 1;

		//������ת��������µĸ��ڵ�
		return child;
	}

	//����ת���� ��node�ڵ�Ϊ��������ת�����������µĸ��ڵ㷵��
	Node* leftRotate(Node* node)
	{
		Node* child = node->right_;
		node->right_ = child->left_;
		child->left_ = node;
		//�߶ȸ���
		node->height = max(height(node->left_), height(node->right_)) + 1;
		child->height = max(height(child->left_), height(child->right_)) + 1;

		//������ת��������µĸ��ڵ�
		return child;
	}

	//��ƽ����� �Բ���node��������Ϊ��������ת������Ȼ���node���ڵ�������ת����
	Node* leftBalance(Node* node) {
		node->left_ = leftRotate(node->left_);
		return rightRotate(node);
	}

	//��ƽ����� �Բ���node�����ұߺ���Ϊ��������ת������Ȼ������node�ڵ�������ת����
	Node* rightBalance(Node* node) {
		node->right_ = rightRotate(node->right_);
		return leftRotate(node);
	}

	Node* insert(Node* node, const T& val) {
		if (node == nullptr) { //�ݹ��ҵ��˲����λ��
			return new Node(val);
		}
		if (node->data_ > val) {
			node->left_ = insert(node->left_, val);
			//���1 �ڵݹ����ʱ�жϽڵ��Ƿ�ʧ�� node��������̫���� nodeʧ����
			if (height(node->left_) - height(node->right_) > 1) {
				//�ڵ�ʧ�� ���ӵ�������̫����
				if (height(node->left_->left_) >= height(node->left_->right_)) {
					node = rightRotate(node);
				}
				else
				{
					//���ӵ�������̫����
					node = leftBalance(node);
				}
			}
		}
		else if (node->data_ < val) {
			node->right_ = insert(node->right_, val);
			//���2 �ڵݹ����ʱ�� �жϽڵ��Ƿ�ʧ�� node��������̫����nodeʧ����
			if (height(node->right_) - height(node->left_) > 1) {
				//�ڵ�ʧ�� �Һ��ӵ�������̫����
				if (height(node->right_->right_) >= height(node->right_->left_)) {
					node = leftRotate(node);
				}
				else
				{
					//�Һ��ӵ�������̫����
					node = rightBalance(node);
				}
			}
		}
		else
		{
			; //�ҵ���ͬ�ڵ��ˣ��������µݹ��ˣ�ֱ�����ϻ���
		}
		//���3 ��������Ϊ�������µĽڵ� �ڵݹ����ʱ������½ڵ�߶�
		node->height = max(height(node->left_), height(node->right_)) + 1;
		return node;
	}

	Node* remove(Node* node, const T& val) {
		if (node == nullptr) {
			return nullptr;
		}
		if (node->data_ > val) {
			node->left_ = remove(node->left_, val);
			//���1 ������ɾ���ڵ㣬�������������̫����
			if (height(node->right_) - height(node->left_) > 1) {
				if (height(node->right_->right_) >= height(node->right_->left_)) {
					//�Һ��ӵ�������̫����
					node = leftRotate(node);
				}
				else
				{
					//�Һ��ӵ�������̫����
					node = rightBalance(node);
				}
			}
		}
		else if (node->data_ < val) {
			node->right_ = remove(node->right_, val);
			//���2 ������ɾ���ڵ� ���ܵ���������̫����
			if (height(node->left_) - height(node->right_) > 1) {
				//���ӵ�����������
				if (height(node->left_->left_) >= height(node->left_->right_)) {
					node = rightRotate(node);
				}
				else
				{
					//���ӵ�����������
					node = leftBalance(node);
				}
			}
		}
		else
		{
			//�ҵ���
			//�ȴ������������ӽڵ��ɾ������
			if (node->left_ != nullptr && node->right_ != nullptr) {
				//���3 Ϊ�˱���ɾ��ǰ�����ߺ�̽ڵ���ɽڵ�ʧ�⣬��˭��ɾ��˭
				if (height(node->left_) > height(node->right_)) {
					//ɾ��ǰ��
					Node* pre = node->left_;
					while (pre->right_ != nullptr) {
						pre = pre->right_;
					}
					node->data_ = pre->data_;
					node->left_ = remove(node->left_, pre->data_); //ɾ��ǰ���ڵ�
				}
				else
				{
					//ɾ�����
					Node* post = node->right_;
					while (post->left_ != nullptr) {
						post = post->left_;
					}
					node->data_ = post->data_;
					node->right_ = remove(node->right_, post->data_);//ɾ����̽ڵ�
				}
			}
			else
			{
				//ɾ���ڵ������һ��
				if (node->left_ != nullptr) {
					Node* left = node->left_;
					delete node;
					return left;
				}
				else if (node->right_ != nullptr) {
					Node* right = node->right_;
					delete node;
					return right;
				}
				else {
					delete node;
					return nullptr;
				}
			}

		}

		//���½ڵ�߶�
		node->height = max(height(node->left_), height(node->right_)) + 1;


		return node; //���ݹ����У�����ǰ�ڵ���丸�ڵ㷵��

	}
	Node* root_;
};




int main() {
	AVLTree<int> avl;
	for (int i = 1; i <= 10; i++) {
		avl.insert(i);
	}
	avl.remove(9);
	avl.remove(10);
	avl.remove(6);
	avl.remove(1);
	avl.remove(2);
	avl.remove(3);

	return 0;
}
#endif