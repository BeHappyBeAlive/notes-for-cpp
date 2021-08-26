#include<iostream>
#include<functional>
#include<stack>
#include<queue>
#include<vector>
#include<cmath>
using namespace std;


template<typename T, typename Compare = less<T>>
class BSTree
{
public:
	//初始化根节点和函数对象
	BSTree(Compare comp=Compare()) 
		:root_(nullptr),compare_(comp) 
	{}
	//层序遍历思想释放BST树所有的节点资源
	~BSTree()
	{
		if (root_ != nullptr) {
			queue<TreeNode*> s;
			s.push(root_);
			while (!s.empty()) {
				TreeNode* front = s.front();
				s.pop();
				if (front->leftchild_ != nullptr)
				{
					s.push(front->leftchild_);
				}
				if (front->rightchild_ != nullptr)
				{
					s.push(front->rightchild_);
				}
				delete front;
			}
		}
	}
	//非递归插入操作
	void non_insert(const T& val)
	{
		//树为空，生成根节点
		if (root_ == nullptr) {
			root_ = new TreeNode(val);
			return;
		}
		//搜索合适的插入位置，记录父节点的位置
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
	//递归插入操作
	void insert(const T& val) {
		//递归需要回溯，所以需要其上一个节点将其连接起来
		//将当前节点给其父节点返回
		root_ = insert(root_, val);
	}
	//非递归删除操作
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

		//未找到待删除节点
		if (cur == nullptr) {
			return;
		}

		//找到待删除节点
		//情况3 待删除节点有两个孩子
		if (cur->leftchild_ != nullptr && cur->rightchild_ != nullptr) {
			TreeNode* pre = cur->leftchild_;
			//找其直接前驱 即左边子树中最大的那个节点
			while (pre->rightchild_ != nullptr) {
				parent = pre;
				pre = pre->rightchild_;
			}
			cur->data_ = pre->data_;
			//让cur指向前驱节点，转化成情况1和2
			cur = pre;
		}
		//cur指向删除节点，parent指向其父节点，
		//统一处理cur指向的节点，情况1和2
		TreeNode* child = cur->leftchild_;
		if (child == nullptr) {
			child = cur->rightchild_;
		}
		if (parent == nullptr) { //说明删除的是根节点
			root_ = child;
		}
		else 
		{
			//把待删除节点的孩子(nullptr或者不为空)写入到其父节点相应的地址域当中
			if (parent->leftchild_ == cur) {
				parent->leftchild_ = child;
			}
			else {
				parent->rightchild_ = child;
			}
		}
		
		delete cur;//删除当前节点
	}
	//递归删除操作
	void remove(const T& val) {
		remove(root_, val);
	}
	//非递归的查询操作
	bool non_query(const T& val) {
		if (root_ == nullptr) {
			return false;
		}
		TreeNode* cur = root_;
		while (cur != nullptr) {
			if (cur->data_ == val) {
				//找到了
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
		//没有找到
		return false;
	}
	//递归的查询操作
	bool query(const T& val) {
		return nullptr != query(root_, val);
	}
	//递归前序遍历
	void preOrder() {
		cout << "递归前序遍历："  ;
		preOrder(root_);
		cout << endl;
	}
	//非递归前序遍历操作
	void non_preOrder() {
		cout << "非递归前序遍历：";
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
	//递归中序遍历
	void inOrder() {
		cout << "递归中序遍历：" ;
		inOrder(root_);
		cout << endl;
	}
	//非递归中序遍历
	void non_inOrder() {
		cout << "非递归中序遍历：";
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
	//递归后序遍历
	void postOrder() {
		cout << "递归后序遍历："  ;
		postOrder(root_);
		cout << endl;
	}
	//非递归后序遍历 LRV => VRL 用两个栈实现 
	void non_postOrder() {
		cout << "非递归后序遍历：";
		if (root_ == nullptr) {
			return;
		}
		stack<TreeNode*> stk1;
		stack<TreeNode*> stk2;
		stk1.push(root_);
		while (!stk1.empty()) {
			//栈1出栈
			TreeNode* top = stk1.top();
			stk1.pop();

			//栈2入栈
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
	//递归层序遍历操作
	void levelOrder() {
		cout << "递归层序遍历:";
		int h = high(); //树的层数
		for (int i = 0; i < h; ++i) {
			levelOrder(root_, i); //递归层序遍历的实现
		}
		cout << endl;
	}
	//非递归层序遍历操作
	void non_levelOrder() {
		cout << "非递归层序遍历：";
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
	//递归获取层数
	int high() {
		return high(root_);
	}
	//递归求二叉树节点个数
	int number() {
		return number(root_);
	}

	//求满足区间的元素值[i,j]
	void findValues(vector<T>& vec, int i, int j) {
		findValues(root_, vec, i, j);
	}

	//判断二叉树是否是BST树
	bool isBSTree()
	{
		TreeNode* pre = nullptr;
		return isBSTree(root_, pre);
	}

	//判断子树问题
	bool isChildTree(BSTree<T, Compare>& child) {
		//在当前二叉树上找child的根节点
		if (child.root_ == nullptr) {
			return true;
		}

		TreeNode* cur = root_;
		while (cur != nullptr) {
			if (cur->data_ == child.root_->data_) {
				//找到了根节点
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
		//然后比较从当前节点开始，
		//和提供的子树的根节点进行比较，比较其中的每一个元素
		return isChildTree(cur, child.root_);

	}

	//判断LCA问题
	int getLCA(T val1, T val2) {
		TreeNode* node = getLCA(root_, val1, val2);
		if (node == nullptr) {
			throw "no LCA!";
		}
		else {
			return node->data_;
		}
	}

	//镜像反转问题
	void mirror01() {
		mirror01(root_);
	}
	bool mirror02() {
		if (root_ == nullptr) {
			return true;
		}
		return mirror02(root_->leftchild_, root_->rightchild_);
	}

	//重建二叉树
	void rebuild(T pre[], int i, int j, T in[], int m, int n) {
		root_ = _rebuild(pre, i, j, in, m, n);
	}

	//判断平衡树
	bool isBanlance() {
		int l = 0;
		bool flag = true;
		isBanlance02(root_, l, flag);
		return flag;
	}

	//求中序倒数第k个节点
	int getVal(int k) {
		TreeNode* node = getVal(root_, k);
		if (node == nullptr)
		{
			string err = "no NO.";
			err += k;
			throw err;
		}
		else
		{
			return node->data_;
		}
	}
private:
	//节点定义
	struct TreeNode
	{
		TreeNode(T data = T())
			:data_(data), leftchild_(nullptr), rightchild_(nullptr)
		{

		}
		T data_; //数据域
		TreeNode* leftchild_; //左孩子域
		TreeNode* rightchild_; //右孩子域

	};
	//前序遍历 VLR
	void preOrder(TreeNode* root) {
		if (root != nullptr) {
			cout << root->data_ << " ";
			preOrder(root->leftchild_);
			preOrder(root->rightchild_);
		}
	}
	//中序遍历 LVR
	void inOrder(TreeNode* root) {
		if (root != nullptr) {
			inOrder(root->leftchild_);
			cout << root->data_ << " ";
			inOrder(root->rightchild_);
		}
	}
	//后序遍历 LRV
	void postOrder(TreeNode* root) {
		if (root != nullptr) {
			postOrder(root->leftchild_);
			postOrder(root->rightchild_);
			cout << root->data_ << " ";
		}
	}
	//递归求BST的层数 求以node为根节点的子树的高度并且返回高度值
	int high(TreeNode* node) {
		if (node == nullptr) {
			return 0;
		}
		int left = high(node->leftchild_);
		int right = high(node->rightchild_);
		return left > right ? left + 1 : right + 1;
	}
	//递归求BST的节点总数 求以node为根节点的树的节点总数，并且返回
	int number(TreeNode* node) {
		if (node == nullptr) {
			return 0;
		}
		int left = number(node->leftchild_);//L
		int right = number(node->rightchild_);//R
		return left + right + 1;//V
	}
	//递归层序遍历实现
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
	//递归实现插入
	TreeNode* insert(TreeNode* node, const T& val) {
		if (node == nullptr) {
			//递归结束，找到插入val的位置，生成新节点，返回其节点
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
	//递归实现查询
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
	//递归实现删除
	TreeNode* remove(TreeNode* node, const T& val) {
		if (node == nullptr) {
			//没找到
			return nullptr;
		}
		
		if (node->data_ == val) {
			//找到待删除节点
			//情况3 两个子节点
			if (node->leftchild_ != nullptr && node->rightchild_ != nullptr) {
				//有两个子节点
				TreeNode* pre = node->leftchild_;
				while (pre->rightchild_ != nullptr) {
					pre = pre->rightchild_;
				}
				//找到前驱节点为pre，将当前节点覆盖掉，用pre的值
				node->data_ = pre->data_;
				//递归删除前驱节点
				node->leftchild_ = remove(node->leftchild_, pre->data_);
			}
			else {//情况1和情况2
				if (node->leftchild_ != nullptr) {//有一个左孩子
					TreeNode* left = node->leftchild_;
					delete node;
					return left;
				}
				else if (node->rightchild_ != nullptr) {//有一个右孩子
					TreeNode* right = node->rightchild_;
					delete node;
					return right;
				}
				else { //删除的是没有孩子的节点 叶子结点
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

	//求满足区间的元素值[i,j]
	void findValues(TreeNode* node, vector<T>& vec, int i, int j) {
		if (node != nullptr) {
			//在当前节点的左子树中搜索
			if (node->data_ > i) {
				findValues(node->leftchild_, vec, i, j);//L
			}
			//V
			if (node->data_ >= i && node->data_ <= j) {
				//存储满足区间的值，但是这里将树都遍历完了，
				//可以优化，优化就是根据边界条件判断是否往左右子树探索
				vec.push_back(node->data_);
			}
			//在当前节点的右子树中搜索
			if (node->data_ < j) {
				findValues(node->rightchild_, vec, i, j);//R
			}
		}
	}

	//判断二叉树是否是BST树
	bool isBSTree(TreeNode* node, TreeNode*& pre) {//传引用是因为需要记录当前节点中序遍历的前一个节点的值，
		if (node == nullptr) {					//如果不加则每一次递归回溯的过程中，栈上pre内存的值会不一样
			return true;
		}
		if (isBSTree(node->leftchild_, pre)) {//L 主要判断是递归结束的条件
			return false;
		}
		//V
		if (pre != nullptr)
		{
			if (compare_(node->data_, pre->data_)) { //主要判断是递归结束的条件
				return false;
			}
		}
		pre = node;//更新中序遍历的前驱节点

		return isBSTree(node->rightchild_, pre);//R
		
		/*
		//V
		if (node->leftchild_ != nullptr && compare_(node->data_, node->leftchild_->data_)) {
			return false;
		}
		if (node->rightchild_ != nullptr && compare_(node->rightchild_->data_, node->data_)) {
			return false;
		}
		if (!isBSTree(node->leftchild_)) {//L 判断当前节点的左子树
			return false;
		}
		if (!isBSTree(node->rightchild_)) {//R 判断当前节点的右子树
			return false;
		}
		else {
			return true;
		}*/
		//上面这种方法有些情况不正确，只是局部判断

	}

	//判断子树
	bool isChildTree(TreeNode* father, TreeNode* child) {
		if (father == nullptr && child == nullptr) {
			return true;
		}
		if (father == nullptr) {
			return false; //子树里面有的节点当前节点没有
		}
		if (child == nullptr) {
			return true;	//父树里面的节点比子树节点多，这可以接受
		}
		//判断值不相同
		if (father->data_ != child->data_) {//V
			return false;
		}

		return isChildTree(father->leftchild_, child->leftchild_)//L
			&& isChildTree(father->rightchild_, child->rightchild_);//R
	}

	//获取LCA
	TreeNode* getLCA(TreeNode* node, T& val1, T& val2) {

		if (node == nullptr) {
			return nullptr;
		}
		if (compare_(node->data_, val1) && compare_(node->data_, val2)) {
			return getLCA(node->rightchild_, val1, val2);
		}
		else if (compare_(val1, node->data_) && compare_(val2, node->data_)) {
			return getLCA(node->leftchild_, val1, val2);
		}
		else
		{
			return node;
		}
	}

	//镜像反转
	void mirror01(TreeNode* node) {
		if (node == nullptr) {
			return;
		}

		//前序遍历
		//V
		//翻转树节点的左右孩子节点地址
		TreeNode* tmp = node->leftchild_;
		node->leftchild_ = node->rightchild_;
		node->rightchild_ = tmp;

		mirror01(node->leftchild_);//L
		mirror01(node->rightchild_);//R
	}

	//镜像对称
	bool mirror02(TreeNode* node1, TreeNode* node2) {
		if (node1 == nullptr && node2 == nullptr) {
			return true;
		}
		if (node1 == nullptr || node2 == nullptr) {
			return false;
		}
		if (node1->data_ != node2->data_) {
			return false;
		}
		return mirror02(node1->leftchild_, node2->rightchild_)
			&& mirror02(node1->rightchild_, node2->leftchild_);
	}

	//重建二叉树递归实现 中序遍历和前序遍历
	TreeNode* _rebuild(T pre[], int i, int j, T in[], int m, int n) {
		if (i > j || m > n) {
			return nullptr;
		}

		//创建当前子树的根节点
		TreeNode* node = new TreeNode(pre[i]);//拿前序遍历序列的第一个数字创建子树的根节点
		for (int k = m; k <= n; ++k) {
			if (pre[i] == in[k]) {//在中序遍历中找到子树的根节点的下标k
				node->leftchild_ = _rebuild(pre, i + 1, i + (k - m), in, m, k - 1);
				node->rightchild_ = _rebuild(pre, i + (k - m) + 1, j, in, k + 1, n);
				return node;
			}
		}
		return node;
	}

	//判断BST树是否为平衡树 **这样写效率很低**
	bool isBanlance01(TreeNode* node) {
		if (node == nullptr) {
			return true;
		}
		if (!isBanlance(node->leftchild_))//L
		{
			return false;
		}
		if (!isBanlance(node->rightchild_))//R
		{
			return false;
		}
		int left = high(node->leftchild_);
		int right = high(node->rightchild_);
		return (abs(left - right) <= 1);//V

	}
	
	//判断BST树是否为平衡树 **效率高，在递归过程中记录了节点的高度值**
	//其中l为高度值，高度值需要返回
	int isBanlance02(TreeNode* node, int l, bool& flag) {
		if (node == nullptr) {
			return l;
		}

		int left = isBanlance02(node->leftchild_, l + 1, flag);
		if (!flag) {
			return left;
		}
		int right = isBanlance02(node->rightchild_, l + 1, flag);
		if (!flag)
		{
			return right;
		}

		//V
		if (abs(left - right) > 1)
		{
			flag = false;
		}
		return left > right ? left : right;
	}

	//求中序遍历倒数第k个节点
	int i = 1;
	TreeNode* getVal(TreeNode* node, int k) {
		if (node == nullptr) {
			return nullptr;
		}

		TreeNode* right = getVal(node->rightchild_, k);//R
		if (right != nullptr) {
			return right;
		}
		//V
		if (i++ == k)//在RVL顺序下，找到正数第k个元素
		{
			return node;
		}
		return getVal(node->leftchild_, k);//L
	}

	TreeNode* root_;
	Compare compare_;
};

#if 0
void test01() {//测试是否是BST树
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
void test02() { //测试子树
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
void test03() {//测试LCA
	int arr[] = { 54,24,67,0,34,62,69,5,41,64,78 };
	BSTree<int> bst;
	for (int v : arr) {
		bst.insert(v);
	}
	cout << bst.getLCA(5, 41) << endl;

}
void test04() {//测试镜像反转和镜像对称
	int arr[] = { 54,24,67,0,34,62,69,5,41,64,78 };
	BSTree<int> bst;
	for (int v : arr) {
		bst.insert(v);
	}
	bst.inOrder();
	//翻转之后变成逆序
	bst.mirror01();
	bst.inOrder();
	cout << bst.mirror02() << endl;
}
void test05() {
	BSTree<int> bst;
	int pre[] = { 58,24,0,5,34,41,67,62,64,69,78 };
	int in[] = { 0,5,24,34,41,58,62,64,67,69,78 };
	bst.rebuild(pre, 0, 10, in, 0, 10);
	bst.preOrder();
}
#endif



int main() {
	//test03();
	//test01();
	//test02();
	//test04();
	//test05();
#if 0
	//可以在构造函数中传入一个函数对象，采用传入lambda的方式实现
	using Elm = pair<int, string>;
	using Functor = function<bool(Elm, Elm)>;
	BSTree<Elm, Functor> bst1([](Elm p1, Elm p2)->bool {
		return p1.first > p2.first;
		});
#endif

#if 0
	int arr[] = { 54,24,67,0,34,62,69,5,41,64,78 };
	BSTree<int> bst;
	for (int v : arr) {
		//bst.non_insert(v);
		bst.insert(v);
	}
	cout << bst.isBanlance() << endl;
	bst.non_insert(12);
	cout << bst.isBanlance() << endl;
	bst.inOrder();
	cout << bst.getVal(4) << endl;

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