/*28.01.2017 19:54
Очередь событий. Является оберткой над очередью. Нужна для упрощения интерфейса добавления
и добавления возможности извлечь все произошедшие события.
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

		//задается базовая очередь, на которой будет строится очередь событий
		EventQueue(Queue::QueueType QT = Queue::Heap23);

		~EventQueue();

		void Clear();

		int Length();

		bool Empty();

		void Add(Event* e);

		Event* Remove();

		//возвращает все ближайшие события.
		std::vector<Event*> GetNextEvents();

		//возвращает все события,произошедшие до currentTime
		std::vector<Event*> GetHappened(int currentTime);

		Queue::QueueType Type();

		Event* Top();
	};
}