#include<iostream>
using namespace std;


template<typename T>
class RBTree {
public:
	RBTree()
		:root_(nullptr) {

	}
	//�������
	void insert(const T& val)
	{
		if (root_ == nullptr)
		{
			root_ = new Node(val);
			return;
		}
		Node* parent = nullptr;
		Node* cur = root_;
		while (cur != nullptr) {
			if (cur->data_ > val) {
				//������
				parent = cur;
				cur = cur->left_;
			}
			else if (cur->data_ < val)
			{
				//������
				parent = cur;
				cur = cur->right_;
			}
			else
			{
				//�������ظ�ֵ
				return;
			}
		}
		Node* node = new Node(val, parent, nullptr, nullptr, RED);
		//�²���Ľڵ㶼��Ҷ�ӽڵ㣬����Ϊ��ɫ����Ϊ�Ǻ�ɫ����ƻ����������������
		//��֤ÿһ��·���µĺ�ɫ�ڵ������ͬ
		if (parent->data_ > val)
		{
			parent->left_ = node;
		}
		else
		{
			parent->right_ = node;
		}

		//����²���Ľڵ�ĸ��ڵ�Ҳ�Ǻ�ɫ�Ļ�����ô�Ͳ��������������ʣ����в����������
		if (RED == color(parent))
		{
			fixAfterInsert(node);
		}
	}

	//ɾ������
	void remove(const T& val)
	{
		if (root_ == nullptr)
		{
			return;
		}
		Node* cur = root_;
		while (cur != nullptr)
		{
			if (cur->data_ > val)
			{
				cur = cur->left_;
			}
			else if (cur->data_ < val)
			{
				cur = cur->right_;
			}
			else
			{
				break;
			}
		}

		//û�ҵ�val�ڵ㷵��
		if (cur == nullptr)
		{
			return;
		}

		//ɾ��ǰ���ڵ� �����
		if (cur->left_ != nullptr && cur->right_ != nullptr)
		{
			Node* pre = cur->left_;
			while (pre->right_ != nullptr)
			{
				pre = pre->right_;
			}
			cur->data_ = pre->data_;
			cur = pre;//curָ��ǰ���ڵ�

		}
		//ɾ��curָ��Ľڵ� ���1��2
		Node* child = cur->left_; //��childָ��Ϊ�յĺ���
		if (child == nullptr)
		{
			child = cur->right_;
		}
		//
		if (child != nullptr)
		{
			child->parent_ = cur->parent_;
			if (cur->parent_ == nullptr)
			{
				//˵��cur֮ǰ���Ǹ��ڵ�
				root_ = child;
			}
			else
			{
				if (cur->parent_->left_ == cur)
				{
					cur->parent_->left_ = child;
				}
				else
				{
					cur->parent_->right_ = child;
				}
			}

			Color c = color(cur);
			delete cur;
			if (c == BLACK)
			{
				//ɾ����Ϊ��ɫ�ڵ�
				//��Ҫ����ɾ����������
				fixAfterRemove(child);
			}
		}
		else
		{
			//�������Ϊ��
			if (cur->parent_ == nullptr)
			{
				delete cur;
				root_ = nullptr;
				return;
			}
			else
			{
				//ɾ����cur����Ҷ�ӽڵ���
				if (color(cur) == BLACK)
				{
					fixAfterRemove(cur);
				}

				if (cur->parent_->left_ == cur)
				{
					cur->parent_->left_ = nullptr;
				}
				else
				{
					cur->parent_->right_ = nullptr;
				}
				delete cur;
			}


		}


	}

private:
	//������ڵ����ɫ
	enum Color {
		BLACK,
		RED
	};
	//�ڵ�����
	struct Node {
		Node(T data = T(),
			Node* parent = nullptr,
			Node* left = nullptr,
			Node* right = nullptr,
			Color color = BLACK)
			:data_(data),
			left_(left),
			right_(right),
			parent_(parent),
			color_(color)
		{}
		T data_;
		Node* left_;
		Node* right_;
		Node* parent_;//ָ��ǰ�ڵ�ĸ��׽ڵ�
		Color color_; //�������������ɫ
	};

	//��ȡ�ڵ���ɫ
	Color color(Node* node)
	{
		return node == nullptr ? BLACK : node->color_;
	}

	//���ýڵ����ɫ
	void setColor(Node* node, Color color)
	{
		node->color_ = color;
	}

	//���ؽڵ������
	Node* left(Node* node)
	{
		return node->left_;
	}

	//���ؽڵ���Һ���
	Node* right(Node* node)
	{
		return node->right_;
	}

	//��ȡ�ڵ�ĸ��ڵ�
	Node* parent(Node* node)
	{
		return node->parent_;
	}

	//����ת
	void leftRotate(Node* node)
	{
		Node* child = node->right_;
		child->parent_ = node->parent_;
		if (node->parent_ == nullptr) {
			//node�������Ǹ��ڵ�
			root_ = child;
		}
		else
		{
			if (node->parent_->left_ == node) {
				//node�ڸ��ڵ��������
				node->parent_->left_ = child;
			}
			else
			{
				//node�ڸ��ڵ���Һ�����
				node->parent_->right_ = child;
			}
		}

		node->right_ = child->left_;
		if (child->left_ != nullptr)
		{
			child->left_->parent_ = node;
		}

		child->left_ = node;
		node->parent_ = child;
	}

	//����ת
	void rightRotate(Node* node) {
		Node* child = node->left_;
		child->parent_ = node->parent_;
		if (node->parent_ == nullptr) {
			//˵��node�������Ǹ��ڵ�
			root_ = child;
		}
		else
		{
			if (node->parent_->left_ == node) {
				//node�ڸ��ڵ��������
				node->parent_->left_ = child;
			}
			else
			{
				//node�ڸ��ڵ���Һ�����
				node->parent_->right_ = child;
			}
		}

		node->left_ = child->right_;
		if (child->right_ != nullptr)
		{
			child->right_->parent_ = node;
		}

		child->right_ = node;
		node->parent_ = child;
	}

	//������Ĳ����������
	void fixAfterInsert(Node* node)
	{
		//�����ǰ�ڵ�ĸ��ڵ����ɫҲΪ��ɫ�����������
		while (color(parent(node)) == RED)
		{
			if (left(parent(parent(node))) == parent(node))
			{
				//����Ľڵ�������������
				Node* uncle = right(parent(parent(node)));
				//���1 ����ڵ�Ϊ��ɫ ���׺�����ڵ�Ϊ��ɫ үү�ڵ�Ϊ��ɫ
				//�������׺��������ɫΪ��ɫ үү��ɫΪ��ɫ
				if (RED == color(uncle))
				{
					setColor(parent(node), BLACK);
					setColor(uncle, BLACK);
					setColor(parent(parent(node)), RED);
					//�������ϵ���
					node = parent(parent(node));
				}
				else
				{
					//�ȴ������3
					//��ǰ����ڵ��ڸ��׵��Һ����� ������ɫΪ��ɫ 
					//������ɫΪ��ɫ үү����ɫΪ��ɫ
					if (right(parent(node)) == node)
					{
						//node��λ�ñ䲻ͬ�ˣ���Ҫ�ı�Ϊ�����2һ�������
						node = parent(node);
						//�ٴ���ת֮�󣬾ͱ䵽����������2һ��
						leftRotate(node);
					}

					//ͳһ�������2
					//������ɫ����Ϊ��ɫ
					setColor(parent(node), BLACK);
					//үү��ɫ����Ϊ��ɫ
					setColor(parent(parent(node)), RED);
					//��үү�ڵ�Ϊ������ת
					rightRotate(parent(parent(node)));
					//����·�����ﵽ����������
					//���Բ������Ͻ��в���
					break;

				}
			}
			else
			{
				//����Ľڵ�������������
				Node* uncle = left(parent(parent(node)));
				//���1 ����ڵ�Ϊ��ɫ ���׺�����ڵ�Ϊ��ɫ үү�ڵ�Ϊ��ɫ
				//�������׺��������ɫΪ��ɫ үү��ɫΪ��ɫ
				if (RED == color(uncle))
				{
					setColor(parent(node), BLACK);
					setColor(uncle, BLACK);
					setColor(parent(parent(node)), RED);
					//�������ϵ���
					node = parent(parent(node));
				}
				else
				{
					//�ȴ������3
					//��ǰ����ڵ��ڸ��׵��Һ����� ������ɫΪ��ɫ 
					//������ɫΪ��ɫ үү����ɫΪ��ɫ
					if (left(parent(node)) == node)
					{
						//node��λ�ñ䲻ͬ�ˣ���Ҫ�ı�Ϊ�����2һ�������
						node = parent(node);
						//�ٴ���ת֮�󣬾ͱ䵽����������2һ��
						rightRotate(node);
					}

					//ͳһ�������2
					//������ɫ����Ϊ��ɫ
					setColor(parent(node), BLACK);
					//үү��ɫ����Ϊ��ɫ
					setColor(parent(parent(node)), RED);
					//��үү�ڵ�Ϊ������ת
					leftRotate(parent(parent(node)));
					//����·�����ﵽ����������
					//���Բ������Ͻ��в���
					break;

				}
			}
		}

		//ǿ���ø��ڵ����ɫΪ��ɫ
		setColor(root_, BLACK);
	}

	//�������ɾ����������
	void fixAfterRemove(Node* node)
	{
		while (node != root_ && color(node) == BLACK)//���ɾ����ǰ�ڵ�󣬲������ĺ��ӽڵ�Ϊ��ɫ
		{
			//ɾ���ĺ�ɫ�ڵ���������
			if (left(parent(node)) == node)
			{
				Node* brother = right(parent(node));
				if (color(brother) == RED)//�����
				{
					setColor(parent(node), RED);
					setColor(brother, BLACK);
					leftRotate(parent(node));
					//����brother�ڵ�
					brother = right(parent(node));
				}

				//��֤��ɾ���ڵ���ֵ�Ϊ��ɫ����
				if (color(left(brother)) == BLACK
					&& color(right(brother)) == BLACK)//�����
				{
					setColor(brother, RED);
					node = parent(node);//��ָ��ָ���ף�����������
				}
				else
				{
					if (color(right(brother)) != RED)//���2
					{
						setColor(brother, RED);
						setColor(left(brother), BLACK);
						//���ֵ�Ϊ��������ת����
						rightRotate(brother);
						//����brother
						brother = right(parent(node));

					}
					//��ᵽ���1
					setColor(brother, color(parent(node)));
					setColor(parent(node), BLACK);
					setColor(right(brother), BLACK);
					leftRotate(parent(node));
					break;
				}
			}
			else
			{
				//ɾ���ĺ�ɫ�ڵ���������
				Node* brother = left(parent(node));
				if (color(brother) == RED)//�����
				{
					setColor(parent(node), RED);
					setColor(brother, BLACK);
					rightRotate(parent(node));
					//����brother�ڵ�
					brother = left(parent(node));
				}

				//��֤��ɾ���ڵ���ֵ�Ϊ��ɫ����
				if (color(left(brother)) == BLACK
					&& color(right(brother)) == BLACK)//�����
				{
					setColor(brother, RED);
					node = parent(node);//��ָ��ָ���ף�����������
				}
				else
				{
					if (color(left(brother)) != RED)//���2
					{
						setColor(brother, RED);
						setColor(right(brother), BLACK);
						//���ֵ�Ϊ��������ת����
						leftRotate(brother);
						//����brother
						brother = left(parent(node));

					}
					//��ᵽ���1
					setColor(brother, color(parent(node)));
					setColor(parent(node), BLACK);
					setColor(left(brother), BLACK);
					rightRotate(parent(node));
					break;
				}
			}
		}

		//����������ĺ���Ϊ��ɫ����ֱ�ӽ�������Ϊ��ɫ����������
		setColor(node, BLACK);
	}

	Node* root_;//���ڵ�
};




int main() {

	RBTree<int> rbt;
	for (int i = 1; i <= 10; ++i) {
		rbt.insert(i);
	}



	rbt.remove(9);
	rbt.remove(10);
	rbt.remove(5);
	rbt.remove(3);

	return 0;
}