/*28.01.2017 19:54
Очередь событий. Является оберткой над очередью. Нужна для упрощения интерфейса добавления
и добавления возможности извлечь все произошедшие события.
*/
#pragma once
#include "Queue.cpp"
#include "Event.h"
#include <vector>
namespace Queue
{
	class EventQueue
	{
		IQueue<Model::Event*>* data;
	public:

		//задается базовая очередь, на которой будет строится очередь событий
		EventQueue(QueueType QT = Heap23)
		{
			data = Queue::Create<Model::Event*>(QT);
		}

		~EventQueue()
		{
			delete data;
		}

		void Clear()
		{
			data->Clear();
		}

		int Length()
		{
			return data->Length();
		}

		bool Empty()
		{
			return data->IsEmpty();
		}

		void Add(Model::Event* e)
		{
			data->Add(e->Time, e);
		}

		//возвращает все ближайшие события.
		std::vector<Model::Event*> GetNextEvents()
		{
			std::vector<Model::Event*> happening;
			int now = data->Top()->Time;
			while (data->Length() > 0 && data->Top()->Time == now)
			{
				happening.push_back(data->Remove());
			}
			return happening;
		}

		//возвращает все события,произошедшие до currentTime
		std::vector<Model::Event*> GetHappened(int currentTime)
		{
			std::vector<Model::Event*> happened;
			while (data->Length() > 0 && data->Top()->Time < currentTime)
			{
				happened.push_back(data->Remove());
			}
			return happened;
		}

		Queue::QueueType Type()
		{
			return this->data->Type();
		}
	};
}