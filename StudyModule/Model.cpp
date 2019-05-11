#pragma once
#include "Model.h"
#include <time.h>
#include "DistributionBuilder.h"
namespace Model
{


	//запускает процесс моделирования( непосредственно обработки событий)
	void Model::StartModeling()
	{
		modelStatistic.StartTime = clock();
		while (eventQueue->Empty() == false)
		{
			std::vector<Event*> currentEvents = this->eventQueue->GetNextEvents();
			currentTime = currentEvents[0]->Time;
			for each (Event* currentEvent in currentEvents)
			{
				HandleEvent(currentEvent);
				delete currentEvent;
			}
		}
		modelStatistic.FinishTime = clock();
		modelStatistic.ModelTime = this->currentTime;
	}

	//обработка события happened
	void Model::HandleEvent(Event* happened)
	{
		switch (happened->Id)
		{
		case Free:
			EventEnter(happened->Sender);
			break;
		case Processed:
			EventRelease(happened->Sender);

			break;
		}
	}

	//обработка события первого типа для транзакта entering
	void Model::EventEnter(Transact* entering)
	{
		//1..n -> транзакт в канале
		//n + 1 -> транзакт в терминаторе
		int newNode = entering->Move(this->network);

		//транзакт попал в терминал -> нужно пометить что транзакт обработан
		if (newNode > network->Channel.size())
		{
			entering->OnStop(this->currentTime);
		}
		//иначе newNode теперь равен каналу, куда перешел транзакт -> пытаемся занять
		else
		{
			Event* e = network->Channel[newNode - 1]->BeginCapture(entering, currentTime);
			//если канал был свободен,и сейчас займется, то породится событие освобождения(С2)
			if (e != nullptr)
				eventQueue->Add(e);
		}
		
	}

	//обработка события второго типа для транзакта releasing
	void Model::EventRelease(Transact* releasing)
	{

		int currentNode = releasing->currentNode;
		//если после освобождения канала в очереди к каналу есть ещё транзакты,то
		//канал будет снова занят,и ородит событие освобождения.
		Event* happened = network->Channel[currentNode - 1]->Release(releasing,this->currentTime);
		if (happened != nullptr)
		{
			eventQueue->Add(happened);
		}
		Event* newEvent = new Event(Free, releasing, currentTime);
		//теперь транзакт освобожден и его необходимо передвинуть далее.
		//EventEnter(releasing);
		eventQueue->Add(newEvent);
	}

	//добавляет транзакт
	void Model::AddTransact()
	{
		int nextTransactTime = generator->Next();
		//следующий транзакт появится после предыдущего, поэтому учитываем время появления предыдущего
		nextTransactTime += transactList.empty() ? 0 : transactList.back()->Statistic.Start;
		Transact* t = new Transact();
		t->OnStart(nextTransactTime);
		transactList.push_back(t);
		eventQueue->Add(new Event(EventId::Free, t, nextTransactTime));
		modelStatistic.TransactAmount++;
	}

	//матрица переходов, генераторы для узлов, генератор модели, очередь событий
	Model::Model(const BasicMatrix<double>& transitionMatrix,
		const std::vector<Node*>& nodeGenerator, Random::IDistribution<double>* modelGenerator,
		const Queue::QueueType& QT) :
		eventQueue(new Queue::EventQueue(QT))
	{

		network = new Network(transitionMatrix, nodeGenerator);
		this->currentTime = 0;
		this->generator = modelGenerator;
		
	}


	Model::~Model()
	{
		Clear();
		delete network;
		delete eventQueue;
		delete generator;
	}

	//запускает процесс поделирования на ammountOf транзактов
	void Model::Start(int ammountOf)
	{
		Clear();
		for (int i = 0; i < ammountOf; i++)
		{
			AddTransact();
		}
		StartModeling();
	}

	//очищает всю статистику модели,а также удаляет транзакты из списка транзактов.
	void Model::Clear()
	{
		modelStatistic.Clear();
		for (int i = 0; i < transactList.size(); i++)
		{
			delete transactList[i];
		}
		transactList.clear();
		for each (Node* node in this->network->Channel)
		{
			node->Clear();
		}
		eventQueue->Clear();
		currentTime = 0;
	}
	//возвращает текущее модельное время
	int Model::GetCurrentTime()
	{
		return currentTime;
	}
	//возвращает накопленную статистику в удобном виде
	Statistic::FullStatistic Model::ComputeStatistic()
	{
		Statistic::FullStatistic state;
		state.ModelInfo = this->modelStatistic;
		int nodeCounter = 0;
		double probabilitySum = 0;
		
		for each (Node* node in this->network->Channel)
		{
			state.LocalStatistic.push_back(Statistic::ModelUnit());
			//средняя длинна очереди
			// = сумма(mi * li) / n
			// li длинна очереди в i состоянии
			// mi длительность i состояния
			// n количество времени
			//загруженность канала ро
			//количество тактов, когда устройство занято / сколько всего тактов
			long double temp = 0;
			double temp2 = 0;
			double temp3 = 0;
			int lastNotNull = 0;

			for (int i = 1; i < node->Statistic.size(); i++)
			{
				//после каждой компоненты делим на время чтобы снизить вероятность переполнения типа
				if (node->Statistic[i].Time == node->Statistic[i - 1].Time)
				{
					temp += (node->Statistic[i].Time - node->Statistic[lastNotNull].Time) *(node->Statistic[i].QueueLength - node->Statistic[i - 1].QueueLength);
					temp3 += (node->Statistic[i].Time - node->Statistic[lastNotNull].Time) *(node->Statistic[i].QueueLength - node->Statistic[i - 1].QueueLength)
						+ (node->Statistic[i].Time - node->Statistic[lastNotNull].Time) * node->Statistic[i - 1].Busy;
				}
				else
				{
					lastNotNull = i;
					temp += ((node->Statistic[i].Time - node->Statistic[i - 1].Time) * node->Statistic[i - 1].QueueLength);
					temp3 += ((node->Statistic[i].Time - node->Statistic[i - 1].Time) * node->Statistic[i - 1].QueueLength)
						+ (node->Statistic[i].Time - node->Statistic[i - 1].Time) * node->Statistic[i - 1].Busy;
				}
				//считаем количество тактов, которое канал работает
				if (node->Statistic[i - 1].Busy > 0)
				{
					//время,которое канал находился в ожном состоянии * процент загрузки канала
					temp2 += (node->Statistic[i].Time - node->Statistic[i - 1].Time)*((double)node->Statistic[i - 1].Busy / (double)node->Channel());
				}
			}
	
			state.LocalStatistic[nodeCounter].Length = temp / double(currentTime);
			state.LocalStatistic[nodeCounter].Probability = temp2 / double(currentTime);
			state.LocalStatistic[nodeCounter].TransactAmount = temp3 / double(currentTime);

			probabilitySum += state.LocalStatistic[nodeCounter].Probability;
			//среднее время ожидания w
			// = сумма(такт захвата - такт входа)
			//среднее время пребывани в системе u
			// = сумма(такт освобождения - такт входа
			temp = 0;
			temp2 = 0;
			//it - пара <номер транзакта,Рабочая статистика данного транзакта>
			// Рабочая статистика - такт входа, захвата, освобождения
			for each (auto it in node->WorkStatistic)
			{
				temp += (it.Start - it.Enter)/double(node->WorkStatistic.size());
				temp2 += (it.Release - it.Enter)/double(node->WorkStatistic.size());
			}
			state.LocalStatistic[nodeCounter].WaitTime = temp;
			state.LocalStatistic[nodeCounter].ProcessingTime = temp2;
		//	state.LocalStatistic[nodeCounter].TransactAmount =
		//		state.LocalStatistic[nodeCounter].Probability + state.LocalStatistic[nodeCounter].Length;

				//state.LocalStatistic[nodeCounter].WaitTime / state.LocalStatistic[nodeCounter].ProcessingTime;

			nodeCounter++;
		}

		//ещё раз для расчета относительной занятости и коэффициента передачи
		for (int i = 0; i < state.LocalStatistic.size(); i++)
		{
			//state.LocalStatistic[i].TransferCoefficient = state.LocalStatistic[i].Probability / probabilitySum;
			state.LocalStatistic[i].TransferCoefficient = (double)this->network->Channel[i]->WorkStatistic.size() / (double)transactList.size();
		}
		

		for (int i = 0; i < state.LocalStatistic.size(); i++)
		{
			state.GlobalStatistic.Length += state.LocalStatistic[i].Length;
			//	* state.LocalStatistic[i].TransferCoefficient;
			state.GlobalStatistic.Probability += state.LocalStatistic[i].Probability;
			state.GlobalStatistic.TransactAmount += state.LocalStatistic[i].TransactAmount;
			state.GlobalStatistic.ProcessingTime += state.LocalStatistic[i].ProcessingTime
				* state.LocalStatistic[i].TransferCoefficient;
			state.GlobalStatistic.WaitTime += state.LocalStatistic[i].WaitTime;
				//* state.LocalStatistic[i].TransferCoefficient;
		
		}
		

		return state;
	}

	Descriptors::ModelDescriptor Model::GetDescriptor() const
	{
		return Descriptors::ModelDescriptor(this->network->GetDescriptor(),
			Descriptors::DistributionDescriptor(this->generator->Type(), this->generator->Argument()),
			this->eventQueue->Type());
	}
	


}