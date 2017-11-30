/*
18.01.2017 14:26
Реализация Бинарной кучи
*/
#pragma once
#include "binaryHeap.h"
namespace Heap
{
	template<class T>
	BinaryHeap<T>::BinaryHeap(const std::list<int> & vals, const compFunc& comp = Heap::KeyMin)
	{
		compare = comp;
		for each(int it in vals)
			insert(it, 0);
	}

	template<class T>
	void BinaryHeap<T>::heapify(int i)
	{
		//функция просеивает элемент.работает до тех пор
		//пока оба сыновья не будут менее приоритетный чем i
		//левый сын : 2*i+1, правый сын : 2*i+2


		size_t left = _leftChild_(i);
		size_t right = _rightChild_(i);
		size_t curr = i;
		//так как right самый правый
		//пока хотя бы один сын более приоритетнее чем родитель
		//меняемся с ними местами
		while (true)
		{
			left = _leftChild_(i);
			right = _rightChild_(i);
			curr = i;
			if (left < data.size())
			{
				if (compare(data[left].key(), data[i].key()))
					i = left;
			}
			if (right < data.size())
			{
				if (compare(data[right].key(), data[i].key()))
					i = right;
			}
			if (i == curr)
				break;
			std::swap(data[i], data[curr]);

		}
	}
	template <class T>
	BinaryHeap<T>::BinaryHeap(const std::vector<BinaryNode<T> >& vals, const compFunc& comp = Heap::KeyMin)
	{
		compare = comp;
		data = vals;
		int mid = data.size() / 2;
		for (int i = mid; i >= 0; i--)
			heapify(i);
	}

	template <class T>
	BinaryHeap<T>::BinaryHeap(const BinaryHeap<T>& copy)
	{
		compare = copy.compare;
		data = copy.data;
	}
	template <class T>
	BinaryNode<T> BinaryHeap<T>::operator[](const int& i)
	{
		return data[i];
	}
	template <class T>
	void BinaryHeap<T>::editKey(int index, const int& newkey, const T& new_val)
	{
		//изменяем значение на новой
		BinaryNode<T> old = data[index];
		data[index] = BinaryNode<T>(newkey, new_val);
		//если новое значение приоритетнее старого то просеиваем вверх, иначе вниз
		if (compare(newkey, old.key()))
		{
			//пока сын приоритетнее отца меняем его местами с отцом
			while (compare(newkey, data[_parent_(index)].key()) && index != 0)
			{
				std::swap(data[index], data[_parent_(index)]);
				index = _parent_(index);
			}
		}
		else
		{
			heapify(index);
		}
	}
	template <class T>
	int BinaryHeap<T>::extractKey()
	{
		BinaryNode<T> rez = data[0];
		data[0] = data.back();
		heapify(0);
		data.pop_back();
		return rez.key();
	}
	template <class T>
	T BinaryHeap<T>::extractValue()
	{
		BinaryNode<T> rez = data[0];
		data[0] = data.back();
		heapify(0);
		data.pop_back();
		return rez.value();
	}
	template <class T>
	BinaryNode<T> BinaryHeap<T>::extract()
	{
		BinaryNode<T> rez = data[0];
		data[0] = data.back();
		heapify(0);
		data.pop_back();
		return rez;
	}
	template <class T>
	BinaryNode<T> BinaryHeap<T>::getData(const int& index)
	{
		return data[index];
	}
	template <class T>
	std::vector<BinaryNode<T> > BinaryHeap<T>::getData()
	{
		return data;
	}
	template <class T>
	int BinaryHeap<T>::insert(const int& key, const T& value)
	{
		//вставляем новый узел
		data.push_back(BinaryNode<T>(key, value));
		//просеиваем вверх пока узел не будет менее приоритетный родителя или не станет корнем
		int i = data.size() - 1;
		while (i != 0 && compare(data[i].key(), data[_parent_(i)].key()))
		{
			std::swap(data[i], data[_parent_(i)]);
			i = _parent_(i);
		}
		return i;
	}
	template <class T>
	void BinaryHeap<T>::merge(BinaryHeap<T>& sum)
	{
		this->data.insert(this->data.begin(), sum.data.begin(), sum.data.end());
		*this = BinaryHeap(this->data, this->compare);
		sum.data.clear();
	}
	template <class T>
	BinaryNode<T> BinaryHeap<T>::priority()
	{
		return data[0];
	}
	template <class T>
	size_t BinaryHeap<T>::size()
	{
		return data.size();
	}
	template <class T>
	void BinaryHeap<T>::walk(const walkFunc& action)
	{
		for (int i = 0; i < data.size(); i++)
		{
			action(data[i]);
		}
	}
	template <class T>
	void BinaryHeap<T>::copy(BinaryHeap<T>& sum)
	{
		this->data.insert(this->data.begin(), sum.data.begin(), sum.data.end());
		*this = BinaryHeap(this->data, this->compare);
	}
	template <class T>
	void BinaryHeap<T>::clear()
	{
		data.clear();
	}
	template <class T>
	BinaryHeap<T> BinaryHeap<T>::createHeapIf(const unarPred& f)
	{
		std::vector<BinaryNode<T> > new_nodes;
		for (size_t i = 0; i < data.size(); i++)
		{
			if (f(data[i]))
				new_nodes.push_back(data[i]);
		}
		BinaryHeap<T> new_heap(new_nodes, compare);
		return new_heap;
	}
	template <class T>
	bool BinaryHeap<T>::isEmpty()
	{
		return data.size() == 0;
	}
}