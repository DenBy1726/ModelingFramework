/*
21.01.2017 20:08
Очередь без приоритета по дисциплине FIFO(first in first out)
Ключ в методе добавления игнорируется. Реализовано на std::queue<T>
*/
#pragma once
#include "IQueue.h"
#include <queue>

namespace Queue
{

	template <class T>
	class FIFOQueue : public IQueue<T>
	{
	private:
		std::queue<T> _data;
	public:

		FIFOQueue()
		{
			_type = QueueType::FIFO;
			
		}


		//добавляет в очередь запись T с ключом int
		void Add(int key, T value)
		{
			_data.push(value);
		}

		//извлекает из очереди запись T
		T Remove()
		{
			T temp = _data.front();
			_data.pop();
			return temp;
		}

		//возвращает количетсво элементов d очереди
		virtual size_t Length()
		{
			return _data.size();
		}

		//возвращает флаг, определяющий пуста ли очередь
		virtual bool IsEmpty()
		{
			return _data.empty();
		}

		//возвращает самый первый элемент очереди
		virtual T Top()
		{
			return _data.front();
		}

		QueueType Type()
		{
			return _type;
		}

		//В качестве очистки используется присвоение ссылки на новый объект
		void Clear()
		{
			_data = std::queue<T>();
		}

	};
}