/*
21.01.2017 20:08
������� ��� ���������� �� ���������� FIFO(first in first out)
���� � ������ ���������� ������������. ����������� �� std::queue<T>
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


		//��������� � ������� ������ T � ������ int
		void Add(int key, T value)
		{
			_data.push(value);
		}

		//��������� �� ������� ������ T
		T Remove()
		{
			T temp = _data.front();
			_data.pop();
			return temp;
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
			return _data.front();
		}

		QueueType Type()
		{
			return _type;
		}

		//� �������� ������� ������������ ���������� ������ �� ����� ������
		void Clear()
		{
			_data = std::queue<T>();
		}

	};
}