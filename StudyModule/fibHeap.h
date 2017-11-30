/*
18.01.2017 15:13
Куча Фибоначчи.
Идея реализации основана на представлении куи в виде связного корневого списка, и фибоначчиевых деревьев, которые растут
от корневого списка.
Подробнее смотрите в Кормене
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
			//Запускаем от корня рекурсивное удаление
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


		//Функци сливает две кучи в одну. В fb будет NULL
		void merge(FibHeap &fb);


		//Функия извлекает приоритный элемент из кучи
		//Удаление возлагается на пользователя! (посредством оператора delete)
		FibNode<T>* extract();

		int extractKey();

		T extractValue();

		void editKey(FibNode<T>* Node, int new_k, T new_t);

		

	private:
		void walkPart(FibNode<T>* node, bool flag, walkfunc action);

		//не интерфейс!!!
		void clear()
		{
			min = NULL;
			roots_amount = 0;
		}

		//Функция удаляет из кучи узел(предположительно сына) и вставляет заново
		void remove_child(FibNode<T> *Node);

		//Удаляем текущий элемент cur из кольцевого списка братьев 
		//и если отец P(cur) ссылался на этот элемент cur, то перебрасываем 
		//ссылку child элемента P на правого брата cur. Уменьшаем счетчик 
		//количества сыновей у P и делаем метку mark равную true. 
		//Это означет, что у элемента P был удален сын. 
		//Данная метка указывает на то, что если возникнет такая ситуация, 
		//что у одного узла будет удалено более одного сына, то его нужно 
		//поместить в кольцевой список корней. 
		void cascading_cut(FibNode<T> *cur);

		void union_root(FibNode<T>* Node, int nodes_amount);

		//функция связывает соседние элементы от Node как соседей
		// 3 - 4 - 5 -> 3 - 5
		void kill_LR_links(FibNode<T>* Node);

		void add(FibNode<T>* Node, FibNode<T>*& bro, FibNode<T>* par = NULL);

		//функция связывает соседние элементы от Node как соседей относительно корня
		void remove_root(FibNode<T>* Node)
		{
			kill_LR_links(Node);
			roots_amount--;
		}
		//фунция вырезает узел из корневого списка и вставляет его заново
		//ссылки на соседние элементы при этом сохраняются
		void fib_heap_link(FibNode<T>* ch, FibNode<T>* par);

		//Функция балансирует кучу
		void consolidate();

		//Функция перебрасывает всех сыновей данного узла в корень
		void childs_in_root(FibNode<T> *Node)
		{
			while (Node->child)
				remove_child(Node->child);
		}
	};
}
#endif