
#pragma once
#include "EventQueue.h"
#include "Queue.cpp";
namespace Model
{

	//задается базовая очередь, на которой будет строится очередь событий
	EventQueue::EventQueue(Queue::QueueType QT)
	{
		data = Queue::Create<Event*>(QT);
	}

	EventQueue::~EventQueue()
	{
		delete data;
	}

	void EventQueue::Clear()
	{
		data->Clear();
	}

	int EventQueue::Length()
	{
		return data->Length();
	}

	bool EventQueue::Empty()
	{
		return data->IsEmpty();
	}

	void EventQueue::Add(Event* e)
	{
		data->Add(e->Time, e);
	}

	//возвращает все ближайшие события.
	std::vector<Event*> EventQueue::GetNextEvents()
	{
		std::vector<Event*> happening;
		int now = data->Top()->Time;
		while (data->Length() > 0 && data->Top()->Time == now)
		{
			happening.push_back(data->Remove());
		}
		return happening;
	}

	//возвращает все события,произошедшие до currentTime
	std::vector<Event*> EventQueue::GetHappened(int currentTime)
	{
		std::vector<Event*> happened;
		while (data->Length() > 0 && data->Top()->Time < currentTime)
		{
			happened.push_back(data->Remove());
		}
		return happened;
	}

	Queue::QueueType EventQueue::Type()
	{
		return this->data->Type();
	}

	Event* EventQueue::Top()
	{
		return data->Top();
	}

	Event* EventQueue::Remove()
	{
		return data->Remove();
	}
}
