/*
21.01.2017 20:14
ќчередь с приоритетом, реализованна€ на Heap::BinaryHeap<T>
*/
#pragma once
#include "IQueue.h"
#include "binaryHeap.cpp"

namespace Queue
{

	template <class T>
	class BinaryQueue : public IQueue<T>
	{
	protected:
		Heap::BinaryHeap<T> _data;
	public:

		BinaryQueue(Heap::compFunc comp = Heap::KeyMin)
		{
			_type = QueueType::Heap23;
			_data = Heap::BinaryHeap<T>(comp);
		}

		//добавл€ет в очередь запись T с ключом int
		void Add(int key, T value)
		{
			_data.insert(key,value);
		}

		//извлекает из очереди запись T
		T Remove()
		{
			return _data.extractValue();
		}

		//возвращает количетсво элементов d очереди
		virtual size_t Length()
		{
			return _data.size();
		}

		//возвращает флаг, определ€ющий пуста ли очередь
		virtual bool IsEmpty()
		{
			return _data.isEmpty();
		}

		//возвращает самый первый элемент очереди
		virtual T Top()
		{
			return _data.priority().value();
		}

		QueueType Type()
		{
			return _type;
		}

		void Clear()
		{
			_data.clear();
		}
	};
}