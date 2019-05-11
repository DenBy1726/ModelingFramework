#pragma once
#include "Node.h"
#include "FIFOQueue.cpp";
namespace Model
{

	Node::Node(Random::IDistribution<double>* dist, int capacity, Queue::IQueue<Transact*>* q)
	{
		this->generator = dist;
		this->Queue = q;
		this->channel = capacity;
		this->capacity = capacity;
	}


	Node::~Node()
	{
		Clear();
		delete this->Queue;
		delete this->generator;
	}

	std::vector<Statistic::StateStatisticUnit> Node::GetStateStatistic()
	{
		return this->Statistic;
	}

	std::vector<Statistic::WorkStatisticUnit> Node::GetWorkStatistic()
	{
		return this->WorkStatistic;
	}


	//Занят ли канал полностью
	bool Node::IsBusy()
	{
		return capacity == 0;
	}

	//свободная емкость
	int Node::Capacity()
	{
		return capacity;
	}

	//количество каналов
	int Node::Channel()
	{
		return channel;
	}
	//захват канала и порождение события типа С2
	Event* Node::Capture(Transact* haveEntered, const int& time)
	{
		OnStart(haveEntered->Cell, time);
		int timeToProcessed = time + this->generator->Next();
		//событие возникнет к такту: текущее время + время обработки
		return new Event(EventId::Processed, haveEntered, timeToProcessed);
	}

	///вызывается при обработке С1
	//Попытаться занять канал если он свободен,иначе стать в очередь. В случае если транзакт
	//занял канал возвращает событие освобождения канала.
	Event* Node::TryCapture(Transact* haveEntered,const int& time)
	{
		
		Event* happened = nullptr;
		if (this->capacity == 0)
		{
			this->Queue->Add(0, haveEntered);
		}
		else
		{
			happened = Capture(haveEntered, time);
		}
		UpdateState(time);
		return happened;
	}

	//регистрирует зашедший в канал транзакт и начинает попытку занять его
	Event* Node::BeginCapture(Transact* haveEntered, const int& time)
	{
		OnEnter(haveEntered->GetId(), time);
		haveEntered->Cell = WorkStatistic.size() - 1;
		return TryCapture(haveEntered, time);
	}

	void Node::Register(Transact* haveEntered, const int& time)
	{
		OnEnter(haveEntered->GetId(), time);
		haveEntered->Cell = WorkStatistic.size() - 1;
	}


	///Вызывается при обработке С2
	//Освобождает канал и если в канал была загружена новая заявка, возвращает событие
	//освобождения канала
	Event* Node::Release(Transact* released,const int& time)
	{
		Event* willHappen = nullptr;
		OnRelease(released->Cell,time);
		released->Cell = -1;
		//если к каналу есть очередь, то берем следующую заявку, занимаем канал, и возвращаем
		//Событие освобождения канала
		if (Queue->IsEmpty() == false)
		{
			Transact* newTransact = Queue->Remove();
			willHappen = Capture(newTransact, time);
		}
		UpdateState(time);
		return willHappen;
	}


	//Вызывется когда транзакт с номером numberOfTransact вошел в канал
	void Node::OnEnter(const int& id,const int& time)
	{
		WorkStatistic.push_back(Statistic::WorkStatisticUnit());
		WorkStatistic[WorkStatistic.size()-1].Enter = time;
	}


	//Вызывется когда транзакт с номером numberOfTransact начинает обработку
	void Node::OnStart(const int& statId, const int& time)
	{
		WorkStatistic[statId].Start = time;
		this->capacity--;
	}


	//Вызывется когда транзакт с номером numberOfTransact освобождает канал
	void Node::OnRelease(const int& statId, const int& time)
	{
		WorkStatistic[statId].Release = time;
		this->capacity++;
	}

	//Вызывется когда транзакт начинает обработку
	void Node::OnStart(Transact* tr, const int& time)
	{
		WorkStatistic[tr->Cell].Start = time;
		this->capacity--;
	}
	//Вызывется когда транзакт завершает обработку
	void Node::OnRelease(Transact* tr, const int& time)
	{
		WorkStatistic[tr->Cell].Release = time;
		this->capacity++;
	}

	//очистка канала
	void Node::Clear()
	{
		this->Statistic.clear();
		this->WorkStatistic.clear();
		this->capacity = channel;
	}

	//вызывается всякий раз когда изменяется состояние канала или очереди
	void Node::UpdateState(const int& time)
	{
		Statistic::StateStatisticUnit temp;	
		temp.Busy = channel - this->capacity;
		temp.Time = time;
		temp.QueueLength = this->Queue->Length();
		this->Statistic.push_back(temp);
	}

	Descriptors::NodeDescriptor Node::GetDescriptor()
	{
		return Descriptors::NodeDescriptor(Descriptors::DistributionDescriptor(generator->Type(), generator->Argument()),capacity,this->Queue->Type());
	}


}