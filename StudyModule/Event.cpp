#pragma once
#include "Event.h"

namespace Model
{


	Event::Event(const EventId& id, Transact* sender, int time)
	{
		this->Id = id;
		this->Sender = sender;
		this->Time = time;
	}

	Event::Event()
	{

	}

	Event* Event::CreateFreeEvent(Transact* t,int time)
	{
		Event* e = new Event;
		e->Id = Free;
		e->Sender = t;
		e->Time = time;
		return e;
	}

	Event::~Event()
	{
		Time = 0;
	}
}
