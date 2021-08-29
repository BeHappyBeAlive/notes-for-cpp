#include<iostream>
using namespace std;


template<typename T>
class RBTree {
public:
	RBTree()
		:root_(nullptr) {

	}
	//插入操作
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
				//往左走
				parent = cur;
				cur = cur->left_;
			}
			else if (cur->data_ < val)
			{
				//往右走
				parent = cur;
				cur = cur->right_;
			}
			else
			{
				//不插入重复值
				return;
			}
		}
		Node* node = new Node(val, parent, nullptr, nullptr, RED);
		//新插入的节点都是叶子节点，设置为红色，因为是黑色则会破坏整个红黑树的性质
		//保证每一条路径下的黑色节点个数相同
		if (parent->data_ > val)
		{
			parent->left_ = node;
		}
		else
		{
			parent->right_ = node;
		}

		//如果新插入的节点的父节点也是红色的话，那么就不满足红黑树的性质，进行插入调整操作
		if (RED == color(parent))
		{
			fixAfterInsert(node);
		}
	}

	//删除操作
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

		//没找到val节点返回
		if (cur == nullptr)
		{
			return;
		}

		//删除前驱节点 情况三
		if (cur->left_ != nullptr && cur->right_ != nullptr)
		{
			Node* pre = cur->left_;
			while (pre->right_ != nullptr)
			{
				pre = pre->right_;
			}
			cur->data_ = pre->data_;
			cur = pre;//cur指向前驱节点

		}
		//删除cur指向的节点 情况1和2
		Node* child = cur->left_; //让child指向不为空的孩子
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
				//说明cur之前就是根节点
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
				//删除的为黑色节点
				//需要进行删除调整操作
				fixAfterRemove(child);
			}
		}
		else
		{
			//如果孩子为空
			if (cur->parent_ == nullptr)
			{
				delete cur;
				root_ = nullptr;
				return;
			}
			else
			{
				//删除的cur就是叶子节点了
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
	//红黑树节点的颜色
	enum Color {
		BLACK,
		RED
	};
	//节点类型
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
		Node* parent_;//指向当前节点的父亲节点
		Color color_; //代表红黑树的颜色
	};

	//获取节点颜色
	Color color(Node* node)
	{
		return node == nullptr ? BLACK : node->color_;
	}

	//设置节点的颜色
	void setColor(Node* node, Color color)
	{
		node->color_ = color;
	}

	//返回节点的左孩子
	Node* left(Node* node)
	{
		return node->left_;
	}

	//返回节点的右孩子
	Node* right(Node* node)
	{
		return node->right_;
	}

	//获取节点的父节点
	Node* parent(Node* node)
	{
		return node->parent_;
	}

	//左旋转
	void leftRotate(Node* node)
	{
		Node* child = node->right_;
		child->parent_ = node->parent_;
		if (node->parent_ == nullptr) {
			//node本身就是根节点
			root_ = child;
		}
		else
		{
			if (node->parent_->left_ == node) {
				//node在父节点的左孩子上
				node->parent_->left_ = child;
			}
			else
			{
				//node在父节点的右孩子上
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

	//右旋转
	void rightRotate(Node* node) {
		Node* child = node->left_;
		child->parent_ = node->parent_;
		if (node->parent_ == nullptr) {
			//说明node本身就是根节点
			root_ = child;
		}
		else
		{
			if (node->parent_->left_ == node) {
				//node在父节点的左孩子上
				node->parent_->left_ = child;
			}
			else
			{
				//node在父节点的右孩子上
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

	//红黑树的插入调整操作
	void fixAfterInsert(Node* node)
	{
		//如果当前节点的父节点的颜色也为红色，则继续调整
		while (color(parent(node)) == RED)
		{
			if (left(parent(parent(node))) == parent(node))
			{
				//插入的节点在左子树当中
				Node* uncle = right(parent(parent(node)));
				//情况1 插入节点为红色 父亲和叔叔节点为红色 爷爷节点为黑色
				//调整父亲和叔叔的颜色为黑色 爷爷颜色为红色
				if (RED == color(uncle))
				{
					setColor(parent(node), BLACK);
					setColor(uncle, BLACK);
					setColor(parent(parent(node)), RED);
					//继续向上调整
					node = parent(parent(node));
				}
				else
				{
					//先处理情况3
					//当前插入节点在父亲的右孩子上 父亲颜色为红色 
					//叔叔颜色为黑色 爷爷的颜色为黑色
					if (right(parent(node)) == node)
					{
						//node的位置变不同了，需要改变为跟情况2一样的情况
						node = parent(node);
						//再次旋转之后，就变到了下面和情况2一样
						leftRotate(node);
					}

					//统一处理情况2
					//父亲颜色设置为黑色
					setColor(parent(node), BLACK);
					//爷爷颜色设置为红色
					setColor(parent(parent(node)), RED);
					//以爷爷节点为轴右旋转
					rightRotate(parent(parent(node)));
					//两条路径都达到了满足性质
					//所以不再向上进行操作
					break;

				}
			}
			else
			{
				//插入的节点在右子树当中
				Node* uncle = left(parent(parent(node)));
				//情况1 插入节点为红色 父亲和叔叔节点为红色 爷爷节点为黑色
				//调整父亲和叔叔的颜色为黑色 爷爷颜色为红色
				if (RED == color(uncle))
				{
					setColor(parent(node), BLACK);
					setColor(uncle, BLACK);
					setColor(parent(parent(node)), RED);
					//继续向上调整
					node = parent(parent(node));
				}
				else
				{
					//先处理情况3
					//当前插入节点在父亲的右孩子上 父亲颜色为红色 
					//叔叔颜色为黑色 爷爷的颜色为黑色
					if (left(parent(node)) == node)
					{
						//node的位置变不同了，需要改变为跟情况2一样的情况
						node = parent(node);
						//再次旋转之后，就变到了下面和情况2一样
						rightRotate(node);
					}

					//统一处理情况2
					//父亲颜色设置为黑色
					setColor(parent(node), BLACK);
					//爷爷颜色设置为红色
					setColor(parent(parent(node)), RED);
					//以爷爷节点为轴右旋转
					leftRotate(parent(parent(node)));
					//两条路径都达到了满足性质
					//所以不再向上进行操作
					break;

				}
			}
		}

		//强制让根节点的颜色为黑色
		setColor(root_, BLACK);
	}

	//红黑树的删除调整操作
	void fixAfterRemove(Node* node)
	{
		while (node != root_ && color(node) == BLACK)//如果删除当前节点后，补上来的孩子节点为黑色
		{
			//删除的黑色节点在左子树
			if (left(parent(node)) == node)
			{
				Node* brother = right(parent(node));
				if (color(brother) == RED)//情况四
				{
					setColor(parent(node), RED);
					setColor(brother, BLACK);
					leftRotate(parent(node));
					//更新brother节点
					brother = right(parent(node));
				}

				//保证待删除节点的兄弟为黑色的了
				if (color(left(brother)) == BLACK
					&& color(right(brother)) == BLACK)//情况三
				{
					setColor(brother, RED);
					node = parent(node);//让指针指向父亲，继续向上走
				}
				else
				{
					if (color(right(brother)) != RED)//情况2
					{
						setColor(brother, RED);
						setColor(left(brother), BLACK);
						//以兄弟为轴做右旋转操作
						rightRotate(brother);
						//更新brother
						brother = right(parent(node));

					}
					//归结到情况1
					setColor(brother, color(parent(node)));
					setColor(parent(node), BLACK);
					setColor(right(brother), BLACK);
					leftRotate(parent(node));
					break;
				}
			}
			else
			{
				//删除的黑色节点在右子树
				Node* brother = left(parent(node));
				if (color(brother) == RED)//情况四
				{
					setColor(parent(node), RED);
					setColor(brother, BLACK);
					rightRotate(parent(node));
					//更新brother节点
					brother = left(parent(node));
				}

				//保证待删除节点的兄弟为黑色的了
				if (color(left(brother)) == BLACK
					&& color(right(brother)) == BLACK)//情况三
				{
					setColor(brother, RED);
					node = parent(node);//让指针指向父亲，继续向上走
				}
				else
				{
					if (color(left(brother)) != RED)//情况2
					{
						setColor(brother, RED);
						setColor(right(brother), BLACK);
						//以兄弟为轴做右旋转操作
						leftRotate(brother);
						//更新brother
						brother = left(parent(node));

					}
					//归结到情况1
					setColor(brother, color(parent(node)));
					setColor(parent(node), BLACK);
					setColor(left(brother), BLACK);
					rightRotate(parent(node));
					break;
				}
			}
		}

		//如果补上来的孩子为红色，则直接将其设置为黑色，调整结束
		setColor(node, BLACK);
	}

	Node* root_;//根节点
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