/*
21.07.2017 22:16
ќчередь с приоритетом на основе кучи ‘ибоначчи. Heap::FibHeap<T>
*/
#pragma once
#include "IQueue.h"
#include "fibHeap.cpp"

namespace Queue
{

	template<class T>
	class FibonacciQueue : public IQueue<T>
	{
	protected:
		Heap::FibHeap<T> _data;
	public:

		FibonacciQueue(Heap::compFunc comp = Heap::KeyMin)
		{
			_data = Heap::FibHeap<T>(comp);
			_type = QueueType::Fibonacci;
		}

		//добавл€ет в очередь запись T с ключом int
		virtual void Add(int key, T val)
		{
			_data.insert(key, val);
		}

		//извлекает из очереди запись T
		virtual T Remove()
		{
			return _data.extractValue();
		}

		//возвращает количетсво элементов d очереди
		virtual size_t Length()
		{
			return _data.getSize();
		}

		//возвращает флаг, определ€ющий пуста ли очередь
		virtual bool IsEmpty()
		{
			return _data.isEmpty();
		}

		//возвращает самый первый элемент очереди
		virtual T Top()
		{
			return _data.priority()->value();
		}

		QueueType Type()
		{
			return _type;
		}

		void Clear()
		{
			_data = Heap::FibHeap<T>(_data.getFunc());
		}

	};

}