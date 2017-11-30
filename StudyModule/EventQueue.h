/*28.01.2017 19:54
������� �������. �������� �������� ��� ��������. ����� ��� ��������� ���������� ����������
� ���������� ����������� ������� ��� ������������ �������.
*/
#pragma once
#include "IQueue.h"
#include "Event.h"
#include <vector>
namespace Model
{
	class EventQueue
	{
		Queue::IQueue<Event*>* data;
	public:

		//�������� ������� �������, �� ������� ����� �������� ������� �������
		EventQueue(Queue::QueueType QT = Queue::Heap23);

		~EventQueue();

		void Clear();

		int Length();

		bool Empty();

		void Add(Event* e);

		Event* Remove();

		//���������� ��� ��������� �������.
		std::vector<Event*> GetNextEvents();

		//���������� ��� �������,������������ �� currentTime
		std::vector<Event*> GetHappened(int currentTime);

		Queue::QueueType Type();

		Event* Top();
	};
}