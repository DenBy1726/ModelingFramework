/*
21.01.2017 20:14
������� � �����������, ������������� �� Heap::BinaryHeap<T>
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

		//��������� � ������� ������ T � ������ int
		void Add(int key, T value)
		{
			_data.insert(key,value);
		}

		//��������� �� ������� ������ T
		T Remove()
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