/*
18.01.2017 15:13
Куча Фибоначчи.Реализация.
*/
#pragma once
#include "fibHeap.h"
namespace Heap
{
	template<class T>
	void FibNode<T>::remove(bool delete_row)
	{
		//Функция производит удаление узла, и всех связанных с ним узлов

		//1) Рекурсивно проходимся по сыновьям и удаляем их
		if (child)
		{
			child->remove(true);
			delete child;
		}

		//2) Выбираем следующую корневую вершину
		if (delete_row)
		{
			FibNode<T>* cur = right;
			while (cur != this) {
				FibNode<T> * next = cur->right;
				cur->remove(false);
				delete cur;
				cur = next;
			}
		}
	}

	template<class T>
	void FibHeap<T>::walk(walkfunc action)
	{
		if (!min) return;
		walkPart(min, true, action);
		action(min);
	}

	template<class T>
	FibNode<T>* FibHeap<T>::insert(int key, T _val)
	{
		//создаем новый узел и вставяем его в кучу
		FibNode<T>* Node = new FibNode<T>(key, _val);
		add(Node, min);
		return Node;
	}

	template<class T>
	void FibHeap<T>::merge(FibHeap &fb)
	{
		//1) Сливаем относительно корневых элементов
		union_root(fb.min, fb.roots_amount);
		//2) Определяем новый корень
		if (!min || (fb.min && less(fb.min->key(), min->key())))
			min = fb.min;
		fb.clear();
	}

	template<class T>
	FibNode<T>* FibHeap<T>::extract()
	{
		//1) выбираем приоритетный элеент

		FibNode<T>* res = min;

		//2) выбираем следующий приоритетный элемент, для этого:

		//2.1) перебрасываем всех сыновей приритетного в корень, затем вырезаем корень
		//2.2) если куча не пуста балансируем
		if (res)
		{
			childs_in_root(res);
			remove_root(res);
			if (res->right == res)
				min = 0;
			else
			{
				min = min->right;
				consolidate();
			}
		}
		else return NULL;
		return res;
	}

	template<class T>
	int FibHeap<T>::extractKey()
	{
		FibNode<T>* res = min;
		if (res)
		{
			childs_in_root(res);
			remove_root(res);
			if (res->right == res)
				min = 0;
			else {
				min = min->right;
				consolidate();
			}
		}
		else return 0;
		int ans = res->key;
		delete res;
		return ans;
	}

	template<class T>
	T FibHeap<T>::extractValue()
	{
		FibNode<T>* res = min;
		if (res)
		{
			childs_in_root(res);
			remove_root(res);
			if (res->right == res)
				min = 0;
			else {
				min = min->right;
				consolidate();
			}
		}
		else return T();
		T ans = res->value();
		delete res;
		return ans;
	}

	template<class T>
	void FibHeap<T>::editKey(FibNode<T>* Node, int new_k, T new_t)
	{
		//1) устанавливаем новые значения ключа и типа
		Node->_key = new_k;
		Node->_val = new_t;

		//2)если увеличиваем приоритет элемента, и он более приоритетнее отца
		//необходимо его вырезать и снова вставить
		//иначе если отца нет проверить не стал ли он новым приоритетным элементов
		FibNode<T>* par = Node->parent;
		if (par && less(Node->key(), par->key()))
		{
			remove_child(Node);
			cascading_cut(par);
		}
		else if (less(Node->key(), min->key()))
			min = Node;
	}

	template<class T>
	void FibHeap<T>::walkPart(FibNode<T>* node, bool flag, walkfunc action)
	{
		{


			//1) Рекурсивно проходимся по сыновьям
			if (node->child)
			{
				walkPart(node->child, true, action);
				action(node->child);
			}

			//2) Выбираем следующую корневую вершину
			if (flag)
			{
				FibNode<T>* cur = node->right;
				while (cur != node) {
					FibNode<T> * next = cur->right;
					walkPart(cur, false, action);
					action(cur);
					cur = next;
				}
			}
		}
	}

	template<class T>
	void FibHeap<T>::remove_child(FibNode<T> *Node)
	{
		//1) Уменьшаем количество дочерних узлов
		//либо связываем родителя со следующим сыном, либо с NULL
		Node->parent->degree--;
		Node->parent->child = (Node->right == Node ? NULL : Node->right);
		kill_LR_links(Node);
		add(Node, min);
		Node->mark = false;
	}

	template<class T>
	void FibHeap<T>::cascading_cut(FibNode<T> *cur)
	{
		FibNode<T> *par = cur->parent;
		if (par && par->degree)
		{
			if (!par->mark)
				par->mark = true;
			else
			{
				remove_child(cur);
				cascading_cut(par);
			}
		}
	}

	template<class T>
	void FibHeap<T>::union_root(FibNode<T>* Node, int nodes_amount)
	{
		if (Node == NULL) return;
		if (min == NULL)
		{
			min = Node;
			roots_amount = nodes_amount;
		}
		else
		{
			FibNode<T> *L = Node->left;
			FibNode<T> *R = min->right;
			min->right = Node;
			Node->left = min;
			L->right = R;
			R->left = L;
			roots_amount += nodes_amount;
		}
	}

	template<class T>
	void FibHeap<T>::kill_LR_links(FibNode<T>* Node)
	{
		if (Node->left != Node)
		{
			Node->right->left = Node->left;
			Node->left->right = Node->right;
		}
	}

	template<class T>
	void FibHeap<T>::add(FibNode<T>* Node, FibNode<T>*& bro, FibNode<T>* par = NULL)
	{
		if (bro == NULL)
		{
			bro = Node;
			Node->left = Node;
			Node->right = Node;
		}
		else
		{
			Node->right = bro->right;
			Node->right->left = Node;
			Node->left = bro;
			bro->right = Node;
		}
		if (less(Node->key(), bro->key()))
			bro = Node;

		if (bro == min)
		{
			roots_amount++;
			Node->parent = NULL;
		}
		if (par)
		{
			par->degree++;
			Node->parent = par;
		}
	}

	template<class T>
	void FibHeap<T>::fib_heap_link(FibNode<T>* ch, FibNode<T>* par)
	{
		remove_root(ch);
		add(ch, par->child, par);
		ch->mark = false;
	}

	template<class T>
	void FibHeap<T>::consolidate()
	{

		//На каждом шаге будут формироваться новые фибоначчиевы деревья
		//размером равным степени двойки.Если на каком - то этапе 
		//получается два дерева одинакового размера они объединяются
		//в одно и так повторяется до тех пор пока в наборе остаются
		//деревья одинакового размера.

		FibNode<T>* A[31];
		memset(A, 0, sizeof(A));
		FibNode<T>* x = min;
		int init_roots = roots_amount;
		int max_degree = 0;
		for (int i = 0; i < init_roots; ++i)
		{
			int d = x->degree;
			FibNode<T>* next = x->right;
			while (A[d])
			{
				FibNode<T>* y = A[d];
				if (less(y->key(), x->key()))
					swap(x, y);
				fib_heap_link(y, x);
				A[d++] = NULL;
			}
			A[d] = x;
			max_degree = max(max_degree, d);
			x = next;
		}
		min = NULL;
		roots_amount = 0;
		for (int i = 0; i <= max_degree; ++i)
		{
			if (A[i])
			{
				add(A[i], min);
			}
		}
	}


}