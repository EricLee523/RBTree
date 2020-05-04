#include <bits/stdc++.h>
using namespace std;
enum Color{
	RED = 0,
	BLACK
};

// 红黑树节点RB_Node
struct RB_Node{
	int val;
	Color color;
	RB_Node *left;
	RB_Node *right;
	RB_Node *parent;
	RB_Node() = default;
	RB_Node(int _val): val(_val) {
		left = right = parent = NULL;
	}
};

// 红黑树RBTree
class RBTree {
public:
	RB_Node *root;
	void RB_insert(int);
	void RB_erase(int);
	// 编写一个输出函数用于测试,输出前序遍历与中序遍历
	void DisplayTree();
	RBTree(): root(NULL){}

private:
	void PreOrder(RB_Node *node);
	void InOrder(RB_Node *node);
	// 辅助函数用于在插入和删除后通过染色和旋转维护红黑树性质
	void RB_insert_fixup(RB_Node*, RB_Node*&);
	void RB_erase_fixup(RB_Node*, RB_Node*&);
	// 旋转操作
	void LeftRotate(RB_Node* x, RB_Node*&);
	void RightRotate(RB_Node* y, RB_Node*&);
	// 用于删除操作中获得后继节点,此处后继节点选用左子树的最大节点
	RB_Node* TreeSuccessor(RB_Node* x) {
		if (!x) return NULL;
		while (x->right != NULL) {
			x = x->right;
		}
		return x;
	}
};

void RBTree::DisplayTree() {
		cout << "前序遍历: ";
		PreOrder(root);
		cout << endl;
		cout << "中序遍历: ";
		InOrder(root);
	}

void RBTree::PreOrder(RB_Node *node) {
		if (NULL == node) return;
		string color = node->color == RED ? "红" : "黑";
		cout << node->val << "(" << color << ") ";
		PreOrder(node->left);
		PreOrder(node->right);
	}

void RBTree::InOrder(RB_Node *node) {
		if (NULL == node) return;
		InOrder(node->left);
		string color = node->color == RED ? "红" : "黑";
		cout << node->val << "(" << color << ") ";
		InOrder(node->right);
	}


void RBTree::RB_insert(int val) {
	// STEP0: 首先找到要插入的位置,y代表要插入节点的父节点
	RB_Node *z = new RB_Node(val);
	RB_Node *x = root;
	RB_Node *y = NULL;
	while (x != NULL) {
		y = x;
		x = val < x->val ? x->left : x->right;
	}

	// STEP1: 插入该节点并将该节点染成红色,之所以染成红色是因为：若染成黑色,则必定违反性质5,但若染成红色,则可能违反性质4
	// 处理的情况要更少一些
	if (y == NULL) {
		root = z;
	} else if (z->val < y->val)
		y->left = z;
	else
		y->right = z;
	z->parent = y;
	z->color = RED;
	RB_insert_fixup(z, root);
}

void RBTree::RB_insert_fixup(RB_Node *z, RB_Node*& root) {
// 若插入的节点为根节点或者父节点的颜色为黑色，并不需要做任何调整，直接退出循环
	while (z != root && z->parent->color == RED) {
		// 假设父节点是祖父节点的左子树,注意此处祖父节点一定存在
		// 因为若插入为根节点则直接退出，若插入点的父节点为根节点则其color为黑;
		if (z->parent == z->parent->parent->left) {
			RB_Node* y = z->parent->parent->right; // 叔叔节点,可能不存在
			// Case1: 叔叔节点为红色,为了维持性质5,将父节点和叔叔节点同时涂为黑色，祖父节点涂成红色
			// 将当前节点指向祖父节点并开始循环,因为有可能在祖父节点处破坏了性质4
			if (y && y->color == RED) {
				z->parent->color = BLACK;
				y->color = BLACK;
				z->parent->parent->color = RED;
				z = z->parent->parent;
			} else { // 无伯父节点,或伯父节点为黑
				// Case2：若插入节点为父节点的右子树,则以父节点为根进行一次左旋，此时转成Case3
				if (z == z->parent->right) {
					z = z->parent;
					LeftRotate(z, root);
				}
				// 父节点由红涂黑,因此祖父节点涂红,做一次右旋
				z->parent->color = BLACK;
				z->parent->parent->color = RED;
				RightRotate(z->parent->parent, root);
			}
		} else {
		   // 父节点是祖父节点的右子树,此时代码逻辑和前面一样，只需要交换left和right操作即可
			RB_Node* y = z->parent->parent->left;
			if (y && y->color == RED) {
				z->parent->color = BLACK;
				y->color = BLACK;
				z->parent->parent->color = RED;
				z = z->parent->parent;
			} else {
				if (z == z->parent->left) {
					z = z->parent;
					RightRotate(z, root);
				}
				z->parent->color = BLACK;
				z->parent->parent->color = RED;
				LeftRotate(z->parent->parent, root);
			}
		}
	}
	root->color = BLACK;
}
void RBTree::RB_erase(int val) {
   // Step0: 先找到要删除的节点z
	RB_Node *z = root;
	while (z) {
		if (val == z->val)
			break;
		else if (val < z->val)
			z = z->left;
		else
			z = z->right;
	}
	if (!z) return;
	// Step1: 类似AVL树的删除,若z为叶子结点或单孩子节点,则直接用其孩子节点替代它
	// 若z是双孩子节点,则找到其后继节点替代它,注意此处仅是将其后继节点y的数据信息拷贝
	// 到z(不包括颜色),从而没有破坏z处的性质,而转换成删除y的情形，即删除叶子结点或单孩子节点
	RB_Node *y = NULL, *x = NULL;
	if (!z->left || !z->right) {
		y = z;
	} else
	    y = TreeSuccessor(z->left);

	if (y->left)
		x = y->left;
	else
		x = y->right;
	if (x != NULL)
		x->parent = y->parent;
	if (y->parent == NULL) {
		root = x;
	} else if (y == y->parent->left)
		y->parent->left = x;
	else
		y->parent->right = x;
	if (y != z) {
		z->val = y->val;
	}
	// 若x为空,即y是叶子节点,则直接删除就行
	// 否则删除了y,对y的后代节点的性质5可能会产生影响,当y是黑色时
	if (x && y->color == BLACK)
		RB_erase_fixup(x, root);
	delete y;
}

void RBTree::RB_erase_fixup(RB_Node* x, RB_Node*& root) {
	while (x != root && x->color == BLACK) {
		if (x == x->parent->left) {
		   // 此时兄弟节点必不为NULL,若为空则违反性质5,因为原本y这条路有y跟x都是黑色
			RB_Node* w = x->parent->right;
			// Case1: 兄弟节点为红色,染色并右旋,这么做是为了再保持性质5的前提下转换为其余情况
			if (w->color == RED) {
				w->color = BLACK;
				x->parent->color = RED;
				LeftRotate(x->parent, root);
				w = x->parent->right;
			}
			// Case2： 兄弟节点为黑色,且拥有两个黑色孩子(包括NULL),可以这样理解:
			// 将x多余的黑色属性转移到x.p中,但这会导致所有经过w的分支中的黑色节点+1，因此将w染为红色保证性质5
			if ((!w->left || w->left->color == BLACK) && (!w->right && w->right->color == BLACK)) {
				w->color = RED;
				x = x->parent;
			} else {
			  // Case3: 如果w是黑色,且右孩子为黑色,左孩子为红色,那么右旋+染色变成情况4
				if (!w->right || w->right->color == BLACK) {
					w->left->color = BLACK; // RED to BLACK
					w->color = RED;
					RightRotate(w, root);
					w = x->parent->right;
				}
				//Case4: w为黑色,右孩子红色,则左旋一次,为了维持性质5做相应染色操作,此时已经成功解决问题，将x置为根节点
				w->color = x->parent->color;
				x->parent->color = BLACK;
				w->right->color = BLACK;
				LeftRotate(x->parent, root);
				x = root;
			}
		} else {
			RB_Node *w = x->parent->left; // 兄弟节点
			if (w->color == RED) {
				w->color = BLACK;
				x->parent->color = RED;
				RightRotate(x->parent, root);
				w = x->parent->left;
			}
			if ((!w->left || w->left->color == BLACK) || (!w->right || w->right->color == BLACK)) {
				w->color = RED;
				x = x->parent;
			} else {
				if (!w->left || w->left->color == BLACK) {
					w->right->color = BLACK;
					w->color = RED;
					LeftRotate(w, root);
					w = x->parent->left;
				}
				w->color = x->parent->color;
				x->parent->color = BLACK;
				w->left->color = BLACK;
				RightRotate(x->parent, root);
				x = root;
			}
		}
	}
	x->color = BLACK;
}
// 不同于AVL的左右旋,由于红黑树维护了parent指针因此要维护该指针
void RBTree::LeftRotate(RB_Node* x, RB_Node*& root) {
	RB_Node *tmp = x->right;
	x->right = tmp->left;
	if (tmp->left)
		tmp->left->parent = x;
	tmp->parent = x->parent;
	if (x == root)
		root = tmp;
	else if (x == x->parent->left)
		x->parent->left = tmp;
	else
		x->parent->right = tmp;
	tmp->left = x;
	x->parent = tmp;
}

void RBTree::RightRotate(RB_Node *x, RB_Node*& root) {
	RB_Node *tmp = x->left;
	x->left = tmp->right;
	if (tmp->right)
		tmp->right->parent = x;
	tmp->parent = x->parent;
	if (x == root)
		root = tmp;
	else if (x == x->parent->right)
		x->parent->right = tmp;
	else
		x->parent->left = tmp;

	tmp->right = x;
	x->parent = tmp;
}

int main() {
	RBTree rb_tree;
	vector<int> vec{19, 5, 30, 1, 12, 35, 7, 13};
	for (auto ele: vec) {
		rb_tree.RB_insert(ele);
	}
	rb_tree.DisplayTree();

	cout << endl << "Construction Done" << endl;

	cout << "Insert:  " << 6 <<endl;
	rb_tree.RB_insert(6);
	rb_tree.DisplayTree();
    cout << endl;

	vector<int> vec2{12, 5};
	for (auto ele : vec2) {
		rb_tree.RB_erase(ele);
		cout << "Delete:  " << ele <<endl;
		rb_tree.DisplayTree();
		printf("\n---------------------------\n");
	}
	return 0;

}
