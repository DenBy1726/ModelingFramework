/*
21.07.2017 22:16
������� � ����������� �� ������ ���� ���������. Heap::FibHeap<T>
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
			return _data.getSize();
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
			_data = Heap::FibHeap<T>(_data.getFunc());
		}

	};

}