/*
21.01.2017 20:14
ќчередь с приоритетом, реализованна€ на std::priority_queue<std::pair<int,T>>
явл€етс€ min очередью
*/
#pragma once
#include "IQueue.h"
#include <queue>
#include <functional>

namespace Queue
{
	template<class T>
	class less : std::less<int>
	{
	public:
		bool operator()(const std::pair<int, T>& left, const std::pair<int, T>& right)
		{
			if (left.first > right.first)
			{
				return true;
			}
			else
			{
				return false;
			}
		}
	};

	template <class T>
	class PriorityQueue : public IQueue<T>
	{
	private:
		std::priority_queue<std::pair<int, T>, std::vector<std::pair<int, T> >, less<T> > _data;
	public:

		

		PriorityQueue()
		{
			_type = QueueType::Standart;
		}

		//добавл€ет в очередь запись T с ключом int
		void Add(int key, T value)
		{
			_data.push(std::make_pair(key,value));
		}

		//извлекает из очереди запись T
		T Remove()
		{
			std::pair<int, T> temp = _data.top();
			_data.pop();
			return temp.second;
		}

		//возвращает количетсво элементов d очереди
		virtual size_t Length()
		{
			return _data.size();
		}

		//возвращает флаг, определ€ющий пуста ли очередь
		virtual bool IsEmpty()
		{
			return _data.empty();
		}

		//возвращает самый первый элемент очереди
		virtual T Top()
		{
			return _data.top().second;
		}

		QueueType Type()
		{
			return _type;
		}

		//¬ качестве очистки используетс€ присвоение ссылки на новый объект
		void Clear()
		{
			_data = {};
		}
	};
}