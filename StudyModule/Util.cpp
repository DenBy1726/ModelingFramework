/*
05.04.2017 14:50
—труктуры данных дл€ работы с моделью в открытом режиме
*/
#pragma once
#include "IRandom.h"
#include "Generator.h"
#include "IDistribution.h"
#include "Queue.cpp"
#include "StateStatisticUnit.cpp"
#include "WorkStatisticUnit.cpp"
#include "matrix.h"
#include "EventQueue.cpp"
namespace Study
{
	namespace Work
	{
		class Transact;
	}
	namespace Struct
	{
		//класс дл€ хранени€ времени
		struct Time
		{
		public:
			int time = 0;
			Time()
			{

			}
			Time(int val)
			{
				time = val;
			}
			operator int()
			{
				return time;
			}
		};

		//ѕеречисление хранит типы событий
		enum EventId{ Free, Processed };

		//структура дл€ хранени€ событи€.
		//событие хранит тип, врем€ возникновени€ и св€занный транзакт
		struct Event
		{


			EventId Id;
			//Event просто хранит указатель,и не создает его, а следовательно
			//не должен отвечать за уничтожение.
			Work::Transact* Sender;
			int Time;

		};



		


	}

	namespace Work
	{
		class Transact;
		class Node
		{
		public:
			//√енератор задержек
			Random::IDistribution<double>* Generator;
			//ќчередь к каналу
			Queue::FIFOQueue<Transact*> Queue;
			//‘лаг з€н€тости
			bool IsWorking;
			//—татистика —осто€ний
			std::vector<Statistic::StateStatisticUnit> StateStatistic;
			//—татистика работы
			std::vector<Statistic::WorkStatisticUnit> WorkStatistic;

			Node()
			{
				IsWorking = false;
			}

		};

		
		class Network
		{
		public:
			//√енератор дл€ расчета переходов транзактов
			Random::Random Generator;
			//√енератор дл€ определени€ по€влени€ за€вок в сети
			Random::IDistribution<double>* EnterGenerator;
			//матрица переодов
			BasicMatrix<double> Matrix;
			//список каналов сети
			std::vector<Node> Nodes;
			//текущее модельное врем€
			Struct::Time Time;
			//очередь событий
			Queue::PriorityQueue<Struct::Event*> Queue;

			Network()
			{

			}


		};

		class Transact
		{
		public:
			//номер узла в котором сейчас находитс€ транзакт
			int CurrentNode;
			//номер записи, в WorkStatistic в которой зарегистрирован транзакт.
			int Cell;
			//идентификатор транзакта.¬ыдает€с автоматически
			int Id;
			//—татистика транзакта
			Statistic::TransactStatistic Statistic;


			Transact()
			{
				this->Id = -1;
				this->CurrentNode = -1;
				this->Cell = -1;
				//currentNode = 0;//временно
			}



		};


	}

	


}
