#include <bits/stdc++.h>
using namespace std;
enum Color{
	RED = 0,
	BLACK
};

// ������ڵ�RB_Node
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

// �����RBTree
class RBTree {
public:
	RB_Node *root;
	void RB_insert(int);
	void RB_erase(int);
	// ��дһ������������ڲ���,���ǰ��������������
	void DisplayTree();
	RBTree(): root(NULL){}

private:
	void PreOrder(RB_Node *node);
	void InOrder(RB_Node *node);
	// �������������ڲ����ɾ����ͨ��Ⱦɫ����תά�����������
	void RB_insert_fixup(RB_Node*, RB_Node*&);
	void RB_erase_fixup(RB_Node*, RB_Node*&);
	// ��ת����
	void LeftRotate(RB_Node* x, RB_Node*&);
	void RightRotate(RB_Node* y, RB_Node*&);
	// ����ɾ�������л�ú�̽ڵ�,�˴���̽ڵ�ѡ�������������ڵ�
	RB_Node* TreeSuccessor(RB_Node* x) {
		if (!x) return NULL;
		while (x->right != NULL) {
			x = x->right;
		}
		return x;
	}
};

void RBTree::DisplayTree() {
		cout << "ǰ�����: ";
		PreOrder(root);
		cout << endl;
		cout << "�������: ";
		InOrder(root);
	}

void RBTree::PreOrder(RB_Node *node) {
		if (NULL == node) return;
		string color = node->color == RED ? "��" : "��";
		cout << node->val << "(" << color << ") ";
		PreOrder(node->left);
		PreOrder(node->right);
	}

void RBTree::InOrder(RB_Node *node) {
		if (NULL == node) return;
		InOrder(node->left);
		string color = node->color == RED ? "��" : "��";
		cout << node->val << "(" << color << ") ";
		InOrder(node->right);
	}


void RBTree::RB_insert(int val) {
	// STEP0: �����ҵ�Ҫ�����λ��,y����Ҫ����ڵ�ĸ��ڵ�
	RB_Node *z = new RB_Node(val);
	RB_Node *x = root;
	RB_Node *y = NULL;
	while (x != NULL) {
		y = x;
		x = val < x->val ? x->left : x->right;
	}

	// STEP1: ����ýڵ㲢���ýڵ�Ⱦ�ɺ�ɫ,֮����Ⱦ�ɺ�ɫ����Ϊ����Ⱦ�ɺ�ɫ,��ض�Υ������5,����Ⱦ�ɺ�ɫ,�����Υ������4
	// ��������Ҫ����һЩ
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
// ������Ľڵ�Ϊ���ڵ���߸��ڵ����ɫΪ��ɫ��������Ҫ���κε�����ֱ���˳�ѭ��
	while (z != root && z->parent->color == RED) {
		// ���踸�ڵ����游�ڵ��������,ע��˴��游�ڵ�һ������
		// ��Ϊ������Ϊ���ڵ���ֱ���˳����������ĸ��ڵ�Ϊ���ڵ�����colorΪ��;
		if (z->parent == z->parent->parent->left) {
			RB_Node* y = z->parent->parent->right; // ����ڵ�,���ܲ�����
			// Case1: ����ڵ�Ϊ��ɫ,Ϊ��ά������5,�����ڵ������ڵ�ͬʱͿΪ��ɫ���游�ڵ�Ϳ�ɺ�ɫ
			// ����ǰ�ڵ�ָ���游�ڵ㲢��ʼѭ��,��Ϊ�п������游�ڵ㴦�ƻ�������4
			if (y && y->color == RED) {
				z->parent->color = BLACK;
				y->color = BLACK;
				z->parent->parent->color = RED;
				z = z->parent->parent;
			} else { // �޲����ڵ�,�򲮸��ڵ�Ϊ��
				// Case2��������ڵ�Ϊ���ڵ��������,���Ը��ڵ�Ϊ������һ����������ʱת��Case3
				if (z == z->parent->right) {
					z = z->parent;
					LeftRotate(z, root);
				}
				// ���ڵ��ɺ�Ϳ��,����游�ڵ�Ϳ��,��һ������
				z->parent->color = BLACK;
				z->parent->parent->color = RED;
				RightRotate(z->parent->parent, root);
			}
		} else {
		   // ���ڵ����游�ڵ��������,��ʱ�����߼���ǰ��һ����ֻ��Ҫ����left��right��������
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
   // Step0: ���ҵ�Ҫɾ���Ľڵ�z
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
	// Step1: ����AVL����ɾ��,��zΪҶ�ӽ��򵥺��ӽڵ�,��ֱ�����亢�ӽڵ������
	// ��z��˫���ӽڵ�,���ҵ����̽ڵ������,ע��˴����ǽ����̽ڵ�y��������Ϣ����
	// ��z(��������ɫ),�Ӷ�û���ƻ�z��������,��ת����ɾ��y�����Σ���ɾ��Ҷ�ӽ��򵥺��ӽڵ�
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
	// ��xΪ��,��y��Ҷ�ӽڵ�,��ֱ��ɾ������
	// ����ɾ����y,��y�ĺ���ڵ������5���ܻ����Ӱ��,��y�Ǻ�ɫʱ
	if (x && y->color == BLACK)
		RB_erase_fixup(x, root);
	delete y;
}

void RBTree::RB_erase_fixup(RB_Node* x, RB_Node*& root) {
	while (x != root && x->color == BLACK) {
		if (x == x->parent->left) {
		   // ��ʱ�ֵܽڵ�ز�ΪNULL,��Ϊ����Υ������5,��Ϊԭ��y����·��y��x���Ǻ�ɫ
			RB_Node* w = x->parent->right;
			// Case1: �ֵܽڵ�Ϊ��ɫ,Ⱦɫ������,��ô����Ϊ���ٱ�������5��ǰ����ת��Ϊ�������
			if (w->color == RED) {
				w->color = BLACK;
				x->parent->color = RED;
				LeftRotate(x->parent, root);
				w = x->parent->right;
			}
			// Case2�� �ֵܽڵ�Ϊ��ɫ,��ӵ��������ɫ����(����NULL),�����������:
			// ��x����ĺ�ɫ����ת�Ƶ�x.p��,����ᵼ�����о���w�ķ�֧�еĺ�ɫ�ڵ�+1����˽�wȾΪ��ɫ��֤����5
			if ((!w->left || w->left->color == BLACK) && (!w->right && w->right->color == BLACK)) {
				w->color = RED;
				x = x->parent;
			} else {
			  // Case3: ���w�Ǻ�ɫ,���Һ���Ϊ��ɫ,����Ϊ��ɫ,��ô����+Ⱦɫ������4
				if (!w->right || w->right->color == BLACK) {
					w->left->color = BLACK; // RED to BLACK
					w->color = RED;
					RightRotate(w, root);
					w = x->parent->right;
				}
				//Case4: wΪ��ɫ,�Һ��Ӻ�ɫ,������һ��,Ϊ��ά������5����ӦȾɫ����,��ʱ�Ѿ��ɹ�������⣬��x��Ϊ���ڵ�
				w->color = x->parent->color;
				x->parent->color = BLACK;
				w->right->color = BLACK;
				LeftRotate(x->parent, root);
				x = root;
			}
		} else {
			RB_Node *w = x->parent->left; // �ֵܽڵ�
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
// ��ͬ��AVL��������,���ں����ά����parentָ�����Ҫά����ָ��
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
