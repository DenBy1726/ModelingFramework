/*27.01.2017 21:52
Интерфейсная часть транзакта.Транзакт содержит информацию о текущем состоянии заявки,
а также статистику транзакта
Если транзакт покинул сеть, то его текущий узел равен n
Если транзакт был удален, то его текущий узел равен -1
09.02.2017 21:39
Добавлено поле Cell которое является вспомогательным и хранит номер ячейки статистики канала,
в которой транзакт зарегистрирован. Это нужно чтобы упростить процедуру поиска ячейки. Поле открыто,
но в будущем возможно изменение интерфейса.
*/
#pragma once
#include "TransactStatistic.cpp"
#include "Network.h"
namespace Model
{
	class Network;
	class Model;
	class Terminal;
	class Transact
	{
		friend class Generator;
		friend class Terminal;
	protected:
		int currentNode;
		//-1 если транзакт удален
		int id;
		static int g_id;
		
		//Вызывется, когда транзакт появился в сети
		virtual void OnStart(const int& time);
		//вызывается, когда транзакт покинул сеть
		virtual void OnStop(const int& time);

	public:
		Statistic::TransactStatistic Statistic;
		Transact();
		~Transact();
		//получить узел, в котором находится транзакт
		virtual int GetNode();
		//получить ид транзакта
		virtual int GetId();
		//получить количество созданных транзактов за все время
		static int GetAmount();	
		//двигает транзакт по сети matrix и возвращает новый узел, в который перешел транзакт.
		virtual int Move(Network* matrix);
		//номер записи, в которой сейчас зарегистрирован транзакт.
		int Cell;



	};
}

