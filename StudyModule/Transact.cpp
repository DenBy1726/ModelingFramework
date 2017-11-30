#pragma once
#include "Transact.h"

namespace Model
{
	int Transact::g_id = 0;

	Transact::Transact()
	{
		this->id = Transact::g_id++;
		//currentNode = 0;//временно
	}

	Transact::~Transact()
	{
		this->id = -1;
		this->currentNode = -1;
	}

	//получить узел, в котором находится транзакт
	int Transact::GetNode()
	{
		return this->currentNode;
	}

	//получить ид транзакта
	int Transact::GetId()
	{
		return this->id;
	}

	//получить количество созданных транзактов за все время
	int Transact::GetAmount()
	{
		return g_id;
	}

	//Вызывется, когда транзакт появился в сети
	//вызывается моделью
	void Transact::OnStart(const int& time)
	{
		this->Statistic.Start = time;
		currentNode = 0;
	}

	//вызывается, когда транзакт покинул сеть
	//вызывается моделью
	void Transact::OnStop(const int& time)
	{
		this->Statistic.Finish = time;
	}

	//двигает транзакт по сети matrix и возвращает новый узел, в который перешел транзакт.
	int Transact::Move(Network* matrix)
	{
		currentNode = matrix->GetNextChannel(currentNode);
		return currentNode;
	}

}