#include<iostream>
//#define MAX(a,b) a>b?a:b; 
using namespace std;

int MAX(int a, int b)
{
	return a > b ? a : b;
}

/*AVL DataStructs*/
typedef struct AVLNode *AVLTree;
struct AVLNode
{
	int data;
	AVLTree left;
	AVLTree right;
	int height;
};

/*height of tree*/
int getHeight(AVLTree t)
{
	if (!t)
	{
		return 0;
	}
	else
	{
		int hL = 0, hR = 0;
		if (t->left) hL = getHeight(t->left);
		if (t->right) hR = getHeight(t->right);
		return MAX(hL, hR) + 1;
	}

}

/*LL*/
AVLTree singleLRotation(AVLTree A)
{
	AVLTree B = A->left;
	A->left = B->right;
	B->right = A;
	A->height = MAX(getHeight(A->left), getHeight(A->right)) + 1;
	B->height = MAX(getHeight(B->left), getHeight(B->right)) + 1;
	return B;
}
/*RR*/
AVLTree singleRRotation(AVLTree A)
{
	AVLTree B = A->right;
	A->right = B->left;
	B->left = A;
	A->height = MAX(getHeight(A->left), getHeight(A->right)) + 1;
	B->height = MAX(getHeight(B->left), getHeight(B->right)) + 1;
	return B;
}

/*LR*/
AVLTree doubleLRRotation(AVLTree A)
{
	A->left = singleRRotation(A->left);
	return singleLRotation(A);
}

/*RL*/
AVLTree doubleRLRotation(AVLTree A)
{
	A->right = singleLRotation(A->right);
	return singleRRotation(A);
}

/*insert AVLTree*/
AVLTree insertAVL(AVLTree t, int data)
{
	if (!t)
	{
		t = (AVLTree)malloc(sizeof(struct AVLNode));
		t->data = data;
		t->height = 1;
		t->left = NULL;
		t->right = NULL;
	}
	else if (data < t->data)
	{
		t->left = insertAVL(t->left, data);
		if (getHeight(t->left) - getHeight(t->right) == 2)
		{
			if (data < t->left->data)
			{
				/*LL*/
				t = singleLRotation(t);
			}
			else
			{
				/*LR*/
				t = doubleLRRotation(t);
			}
		}
	}
	else if (data > t->data)
	{
		t->right = insertAVL(t->right, data);
		if (getHeight(t->right) - getHeight(t->left) == 2)
		{
			if (data > t->right->data)
			{
				/*RR*/
				t = singleRRotation(t);
			}
			else
			{
				/*RL*/
				t = doubleRLRotation(t);
			}
		}
	}

	t->height = MAX(getHeight(t->left), getHeight(t->right)) + 1;
	return t;
}


int main()
{
	int num;
	cin >> num;
	AVLTree  t = NULL;
	for (int i = 0; i < num; i++)
	{
		int data;
		cin >> data;
		t = insertAVL(t, data);
	}
	cout << t->data;
	return 0;
}