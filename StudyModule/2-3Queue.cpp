/*
21.07.2017 22:21
ќчередь на основе 2-3 кучи Heap::Heap<T>
*/
#pragma once
#include "IQueue.h"
#include "2-3heap.cpp"

namespace Queue
{

	template<class T>
	class Queue23 : public IQueue<T>
	{
	protected:
		Heap::Heap<T> _data;
	public:

		Queue23(Heap::compFunc comp = Heap::KeyMin)
		{
			_data = Heap::Heap<T>(comp);
			_type = QueueType::Heap23;
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
			return _data.priority()->value();
		}

		QueueType Type()
		{
			return _type;
		}

		void Clear()
		{
			_data = Heap::Heap<T>(_data.getFunc());
		}

	};
}