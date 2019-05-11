/*28.01.2017 19:59
»нтерфейсна€ часть модели. ћодель аозвол€ет пользователю управл€ть процессом модеоировани€. ј именно
конфигурировать, запускать, получать статистику.
////////////////////
18.02.2017 15:11
¬место генератора пользователь должен отдавать системе описание генератора, что безопаснее
///////////////////
11.03.2017 11:28
ћодель может вернуть свой описатель,что позволит сохранить и загрузить модель
05.05.2017 13:41
¬строен DistributionBuilder дл€ поддержки пользовательских распределений
*/
#pragma once
#include "ModelStatistic.cpp"
#include "FullStatistic.cpp"
#include "EventQueue.cpp"
#include "IDistribution.h"
#include <vector>
#include "Transact.h"
#include "Network.h"
#include "Event.h"
#include "matrix.h"
#include "DistributionBuilder.h"
#include "Generator.h"
#include <vector>
namespace Model
{
	class Model
	{
		friend class Transact;
		friend class Node;
		int currentTime;
		Statistic::ModelStatistic modelStatistic;
		Queue::EventQueue* eventQueue;
		std::vector<Transact*> transactList;
		Network* network;
		Random::IDistribution<double>* generator;

		//запускает процесс моделировани€( непосредственно обработки событий)
		void StartModeling();
		//обработка событи€ happened
		void HandleEvent(Event* happened);
		//обработка событи€ первого типа дл€ транзакта entering
		void EventEnter(Transact* entering);
		//обработка событи€ второго типа дл€ транзакта releasing
		void EventRelease(Transact* releasing);
		//добавл€ет транзакт
		void AddTransact();

	public:
		Model(const BasicMatrix<double>& transitionMatrix,
			const std::vector<Node*>& nodeGenerator,Random::IDistribution<double>* modelGenerator,
			const Queue::QueueType& QT);


		~Model();
		//запускает процесс поделировани€ на ammountOf транзактов
		void Start(int ammountOf);
		//очищает модель
		void Clear();
		//возвращает текущее модельное врем€
		int GetCurrentTime();
		//возвращает накопленную статистику в удобном виде
		Statistic::FullStatistic ComputeStatistic();

		Descriptors::ModelDescriptor GetDescriptor() const;
		

	};


}