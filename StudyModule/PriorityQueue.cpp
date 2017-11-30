/*
21.01.2017 20:14
������� � �����������, ������������� �� std::priority_queue<std::pair<int,T>>
�������� min ��������
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

		//��������� � ������� ������ T � ������ int
		void Add(int key, T value)
		{
			_data.push(std::make_pair(key,value));
		}

		//��������� �� ������� ������ T
		T Remove()
		{
			std::pair<int, T> temp = _data.top();
			_data.pop();
			return temp.second;
		}

		//���������� ���������� ��������� d �������
		virtual size_t Length()
		{
			return _data.size();
		}

		//���������� ����, ������������ ����� �� �������
		virtual bool IsEmpty()
		{
			return _data.empty();
		}

		//���������� ����� ������ ������� �������
		virtual T Top()
		{
			return _data.top().second;
		}

		QueueType Type()
		{
			return _type;
		}

		//� �������� ������� ������������ ���������� ������ �� ����� ������
		void Clear()
		{
			_data = {};
		}
	};
}