#pragma once
#include "Node.h"
namespace Model
{

	Node::Node(Random::IDistribution<double>* dist, int capacity)
	{
		this->Distribution = dist;
		this->channel = capacity;
		this->capacity = capacity;
	}

	Node::Node(int capacity)
	{
		this->channel = capacity;
		this->capacity = capacity;
	}


	Node::~Node()
	{
		Clear();
		//delete this->Queue;
		//delete this->Distribution;
	}



	//«ан€т ли канал полностью
	bool Node::IsBusy()
	{
		return capacity == 0;
	}

	//свободна€ емкость
	int Node::Capacity()
	{
		return capacity;
	}

	//количество каналов
	int Node::Channel()
	{
		return channel;
	}
	//захват канала и порождение событи€ типа —2
	Event* Node::Capture(Transact* haveEntered, const int& time)
	{
		
		//OnStart(haveEntered->Cell, time);
		this->capacity--;
		int timeToProcessed = time + this->Distribution->Next();
		//событие возникнет к такту: текущее врем€ + врем€ обработки
		return new Event(EventId::Processed, haveEntered, timeToProcessed);
	}

	///вызываетс€ при обработке —1
	//ѕопытатьс€ зан€ть канал если он свободен,иначе стать в очередь. ¬ случае если транзакт
	//зан€л канал возвращает событие освобождени€ канала.
/*	Event* Node::TryCapture(Transact* haveEntered, const int& time)
	{
		
		Event* happened = nullptr;
		if (this->capacity == 0)
		{
			this->Queue->Add(haveEntered);
		}
		else
		{
			happened = Capture(haveEntered, time);
		}
		UpdateState(time);
		return happened;
	}*/
	


	///¬ызываетс€ при обработке —2
	//ќсвобождает канал и если в канал была загружена нова€ за€вка, возвращает событие
	//освобождени€ канала
	void Node::Release()
	{
		if (this->capacity < this->channel)
			this->capacity++;
		//OnRelease(released->Cell,time);
		//если к каналу есть очередь, то берем следующую за€вку, занимаем канал, и возвращаем
		//—обытие освобождени€ канала
	
		//UpdateState(time);

	}


	//¬ызыветс€ когда транзакт с номером numberOfTransact вошел в канал
	

	int Node::GetId()
	{
		return id;
	}




	//очистка канала
	void Node::Clear()
	{
		this->capacity = channel;
	}




}