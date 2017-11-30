/*
12.04.2017 21:21 
Терминал, который считает обработаннве заявки
*/
#pragma once
#include "Transact.h"
#include "Time.h"
namespace Model
{
	class Transact;
	class Terminal
	{
		
		int count;

	public:

		Terminal();

		//получить количество вышедших заявок
		int GetCount();

		//пометить что данный транзакт вышел и системы
		void Terminate(Transact* t, int time);
		void Clear();
	};
}