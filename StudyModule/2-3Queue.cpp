/*
21.07.2017 22:21
������� �� ������ 2-3 ���� Heap::Heap<T>
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

		//��������� � ������� ������ T � ������ int
		virtual void Add(int key, T val)
		{
			_data.insert(key, val);
		}

		//��������� �� ������� ������ T
		virtual T Remove()
		{
			return _data.extractValue();
		}

		//���������� ���������� ��������� d �������
		virtual size_t Length()
		{
			return _data.size();
		}

		//���������� ����, ������������ ����� �� �������
		virtual bool IsEmpty()
		{
			return _data.isEmpty();
		}

		//���������� ����� ������ ������� �������
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