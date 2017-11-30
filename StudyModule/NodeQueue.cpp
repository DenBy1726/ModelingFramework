/*12.04.2017 19:54

*/
#pragma once
#include "NodeQueue.h"
#include "Queue.cpp"
namespace Model
{

	NodeQueue::NodeQueue(Queue::QueueType QT)
	{
		data = Queue::Create<Transact*>(QT);
	}

	NodeQueue::~NodeQueue()
	{
		delete data;
	}

	void NodeQueue::Clear()
	{
		data->Clear();
	}

	int NodeQueue::Length()
	{
		return data->Length();
	}

	bool NodeQueue::Empty()
	{
		return data->IsEmpty();
	}

	void NodeQueue::Add(Transact* e)
	{
		data->Add(e->Statistic.Start, e);
	}

	Transact* NodeQueue::Remove()
	{
		return data->Remove();
	}

	Queue::QueueType NodeQueue::Type()
	{
		return this->data->Type();
	}
}
