/*
18.01.2017 15:13
���� ���������.
���� ���������� �������� �� ������������� ��� � ���� �������� ��������� ������, � ������������� ��������, ������� ������
�� ��������� ������.
��������� �������� � �������
*/
#include "baseHeap.h"
namespace Heap
{
#ifndef _FIBONACCI_HEAP
#define _FIBONACCI_HEAP
#ifndef NULL
#define NULL 0
#endif

	template <class T>
	class FibNode
	{
		template <class T> friend class FibHeap;

		FibNode* parent;
		FibNode* child;
		FibNode* left;
		FibNode* right;
		int degree;
		bool mark;
		int _key;
		T _val;
	public:
		FibNode(int Key, T val) : parent(NULL), child(NULL), left(this), right(this), degree(0), mark(false), _key(Key), _val(val) {}
		FibNode<T>* getParent()
		{
			return parent;
		}
		FibNode<T>* getChild()
		{
			return child;
		}
		FibNode<T> getLeft()
		{
			return left;
		}
		FibNode<T> getRight()
		{
			return right;
		}
		int getDegree()
		{
			return degree;
		}
		bool getMark()
		{
			return mark;
		}
		int key()
		{
			return _key;
		}
		T value()
		{
			return _val;
		}

	private:
		void remove(bool delete_row);
	};

	template <class T>
	class FibHeap
	{
		template <class T> friend class FibNode;

		FibNode<T>* min;
		int roots_amount;
		compFunc less;
	public:
		typedef void(*walkfunc)(FibNode<T>*);
		FibHeap(compFunc cmp = KeyMin) :min(NULL), roots_amount(0), less(cmp){}
#ifdef _LIST_
		FibHeap(std::list<std::pair<int, T> > list, compFunc cmp = baseComp) : min(NULL), roots_amount(0), less(cmp)
		{
			for each(auto it in list)
				insert(it.first, it.second);
		}
		FibHeap(std::list<int> list, compFunc cmp = baseComp) : min(NULL), roots_amount(0), less(cmp)
		{
			for each(auto it in list)
				insert(it, 0);
		}
#endif
		~FibHeap()
		{
			//��������� �� ����� ����������� ��������
			if (!min) return;
			min->remove(true);
			delete min;
		}

		void walk(walkfunc action);


		size_t getSize()
		{
			return roots_amount;
		}
		compFunc getFunc()
		{
			return less;
		}
		bool isEmpty()
		{
			return roots_amount == 0;
		}

		FibNode<T>* insert(int key, T _val);

		FibNode<T>* priority()
		{
			return min;
		}


		//������ ������� ��� ���� � ����. � fb ����� NULL
		void merge(FibHeap &fb);


		//������ ��������� ���������� ������� �� ����
		//�������� ����������� �� ������������! (����������� ��������� delete)
		FibNode<T>* extract();

		int extractKey();

		T extractValue();

		void editKey(FibNode<T>* Node, int new_k, T new_t);

		

	private:
		void walkPart(FibNode<T>* node, bool flag, walkfunc action);

		//�� ���������!!!
		void clear()
		{
			min = NULL;
			roots_amount = 0;
		}

		//������� ������� �� ���� ����(���������������� ����) � ��������� ������
		void remove_child(FibNode<T> *Node);

		//������� ������� ������� cur �� ���������� ������ ������� 
		//� ���� ���� P(cur) �������� �� ���� ������� cur, �� ������������� 
		//������ child �������� P �� ������� ����� cur. ��������� ������� 
		//���������� ������� � P � ������ ����� mark ������ true. 
		//��� �������, ��� � �������� P ��� ������ ���. 
		//������ ����� ��������� �� ��, ��� ���� ��������� ����� ��������, 
		//��� � ������ ���� ����� ������� ����� ������ ����, �� ��� ����� 
		//��������� � ��������� ������ ������. 
		void cascading_cut(FibNode<T> *cur);

		void union_root(FibNode<T>* Node, int nodes_amount);

		//������� ��������� �������� �������� �� Node ��� �������
		// 3 - 4 - 5 -> 3 - 5
		void kill_LR_links(FibNode<T>* Node);

		void add(FibNode<T>* Node, FibNode<T>*& bro, FibNode<T>* par = NULL);

		//������� ��������� �������� �������� �� Node ��� ������� ������������ �����
		void remove_root(FibNode<T>* Node)
		{
			kill_LR_links(Node);
			roots_amount--;
		}
		//������ �������� ���� �� ��������� ������ � ��������� ��� ������
		//������ �� �������� �������� ��� ���� �����������
		void fib_heap_link(FibNode<T>* ch, FibNode<T>* par);

		//������� ����������� ����
		void consolidate();

		//������� ������������� ���� ������� ������� ���� � ������
		void childs_in_root(FibNode<T> *Node)
		{
			while (Node->child)
				remove_child(Node->child);
		}
	};
}
#endif