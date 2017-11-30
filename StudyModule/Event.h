/*27.01.2017 22:07
Содержит данные об одном событии. А именно идентификатор события, транзакт, который
инициировал событие и время возникновения события.
*/
#pragma once
#include "Time.h"
#include "Transact.h"
#include "EventId.h"


namespace Model
{
	class Transact;
	struct Event
	{
		EventId Id;
		//Event просто хранит указатель,и не создает его, а следовательно
		//не должен отвечать за уничтожение.
		Transact* Sender;
		int Time;
		Event(const EventId& id, Transact* sender, int time);

		Event();
		static Event* CreateFreeEvent(Transact* t,int time);

		~Event();
	};
}