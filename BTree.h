#include<iostream>
using namespace std;

template<class K, class V, size_t M>
struct BTreeNode
{
	//�࿪һ���ռ䣬�������
	pair<K, V> _kvs[M];//�ؼ�������
	BTreeNode<K, V, M>* _subs[M+1];//���ӽڵ�
	BTreeNode<K, V, M>* _parent;//����
	size_t size;

	BTreeNode()
		:_parent(NULL)
		, size(0)
	{
		for (size_t i = 0; i < M+1; ++i)
		{
			_subs[i] = NULL;
		}
	}
};

template<class K,class V,size_t M>
class BTree
{
	typedef BTreeNode<K, V, M> Node;
public:
	BTree()
		:_root(NULL)
	{}

	pair<Node*, int> Find(const K& key)
	{
		//Ҫ��������ڵ��������ڵ��е�λ��
		Node* cur = _root;
		Node* parent = NULL;
		while (cur)
		{
			size_t i = 0;
			while (i < cur->size)
			{
				//�ڵ�ǰλ�õ�����
				if (cur->_kvs[i].first > key)
					break;
				else if (cur->_kvs[i].first < key)
				{
					++i;
				}
				else
					return make_pair(cur, i);
			}
			//����������û�ҵ�
			parent = cur;
			cur = cur->_subs[i];
		}
		return make_pair(parent, -1);
	}

	void InSertKV(Node* cur, const pair<K, V>& kv, Node* sub)
	{
		int end = cur->size - 1;
		while (end >= 0)
		{
			if (cur->_kvs[end].first > kv.first)
			{
				//���������±����뵱ǰ�ڵ��±���ͬ�����������±��ǵ�ǰ�ڵ�����+1
				cur->_kvs[end + 1] = cur->_kvs[end];
				cur->_subs[end + 2] = cur->_subs[end + 1];
				--end;
			}
			else
			{
				break;
			}
		}
		//end<0��kv.first>cur_kvs[end].first
		cur->_kvs[end + 1] = kv;
		cur->_subs[end + 2] = sub;
		if (sub)
			sub->_parent = cur;
		cur->size++;
	}

	Node* Divided(Node* cur)
	{
		Node* newNode = new Node;
		int mid = (cur->size) / 2;
		size_t j = 0;
		size_t i = mid + 1;
		for (; i < cur->size; ++i)
		{
			newNode->_kvs[j] = cur->_kvs[i];
			newNode->_subs[j] = cur->_subs[i];
			if (newNode->_subs[j])
				newNode->_subs[j]->_parent = newNode;
			newNode->size++;
			j++;
		}
		//�Һ��ӻ�û��
		newNode->_subs[j] = cur->_subs[i];
		if (newNode->_subs[j])
			newNode->_subs[j]->_parent = newNode;
		return newNode;
	}

	bool InSert(const pair<K, V>& kv)
	{
		//�ڵ�ΪNULLֱ�Ӳ���
		if (_root == NULL)
		{
			_root = new Node;
			_root->_kvs[0] = kv;
			_root->size = 1;
			return true;
		}
		//�ҵ���ֵͬ����false,û�ҵ�����true,�ڵ�Ĺؼ������˾ͽ��з���
		pair<Node*, int> ret = Find(kv.first);
		if (ret.second >= 0)
			return false;

		//û�ҵ������Բ���ڵ�
		Node* cur = ret.first;
		pair<K, V> newKV = kv;//�µĹؼ���
		Node* sub = NULL;

		while (1)
		{
			//����һ���˺��Ӻ�һ���ؼ���
			InSertKV(cur, newKV, sub);
			if (cur->size < M)
				return true;
			else
			{
				//��Ҫ����
				Node* newNode = Divided(cur);
				pair<K, V> midKV = cur->_kvs[(cur->size) / 2];
				//���ڵ����
				cur->size -= (newNode->size + 1);
				if (cur == _root)
				{
					_root = new Node;
					_root->_kvs[0] = midKV;
					_root->size = 1;
					_root->_subs[0] = cur;
					_root->_subs[1] = newNode;
					cur->_parent = _root;
					newNode->_parent = _root;
					return true;
				}
				else
				{
					sub = newNode;
					newKV = midKV;
					cur = cur->_parent;
				}
			}
		}
	}

	void InOrder()
	{
		_InOrder(_root);
	}

protected:
	void _InOrder(Node* root)
	{
		if (root == NULL)
			return;
		Node* cur = root;
		size_t i = 0;
		for (; i < cur->size; ++i)
		{
			_InOrder(root->_subs[i]);
			cout << cur->_kvs[i].first << " ";
		}
		_InOrder(cur->_subs[i]);
	}
private:
	Node* _root;
};

void Test()
{
	int a[] = { 53, 75, 139, 49, 145, 36, 101 };
	int sz = sizeof(a) / sizeof(a[0]);
	BTree<int, int, 3>bt;
	for (size_t i = 0; i < sz; ++i)
	{
		bt.InSert(make_pair(a[i],i));
	}
	bt.InOrder();
}