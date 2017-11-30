#include "Generator.h"
#include "DistributionBuilder.h"
#include "NodeQueue.h"
#include "Node.h"
#include "Statistic.h"
#include "StreamReport.cpp"
#include "RussianLocale.cpp"

using namespace Model;
using namespace Queue;
using namespace Statistic;
int main()
{
	//инициализация сети
	/* 
	______        __________		_________		 _______
	| Г	  \______|	|  О|   |______|	У	 |______/	Т	|
	|_____/		 |__|___|___|	   |_________|		\_______|
	Генератор -> Очередь -> Устройство -> Терминал
	*/
	Generator* generator = new Generator;
	generator->Distribution = Random::Distribution::Create("EXP", { 20 });

	Node* node = new Node(1);
	node->Distribution = Random::Distribution::Create("EXP", { 10 });
	node->Queue = new NodeQueue;
	//node->Queue = node_queue;

	//инициализация матрицы переходов
	Matrix<double> transition(3,3);
	transition[0][1] = 1;
	transition[1][2] = 1;


	Network* network = new Network;
	network->TransitionMatrix = transition;
	network->Channel = new NodeList;
	network->Channel->Add(node);
	network->Generator = generator;
	network->Terminal = new Terminal;
	network->EventQueue = new EventQueue;
	network->Time = new Time(0);



	//инициализация таблиц статистики
	StatisticTable* Table = new StatisticTable(1,network->Channel);
	
	int ammount = 50000;

	//загрузка сети заявками
	std::vector<Transact*> transactList;
	for (int i = 0; i < ammount; i++)
	{
		Transact* t = network->Generator->CreateTransact();
		transactList.push_back(t);
		Event* e = Event::CreateFreeEvent(t,t->Statistic.Start);
		network->EventQueue->Add(e);
	}

	while (network->EventQueue->Empty() == false)
	{
		network->MoveTime();
		Event* currentEvent = network->EventQueue->Remove();

		//получить связанный транзакт
		Transact* transact = currentEvent->Sender;
		Node* node = nullptr;
		Time time = *network->Time;
		switch (currentEvent->Id)
		{

		case Free:
			transact->Move(network);

			if (network->IsTerminator(transact->GetNode()) == true)
			{
				network->Terminal->Terminate(transact, time);
			}
			else
			{
				node = network->Channel->Get(transact->GetNode() - 1);
				Table->Register(node, transact, time);
				if (node->IsBusy() == true)
				{
					node->Queue->Add(transact);
				}
				else
				{
					Table->OnStart(node, transact, time);
					Event* happened = node->Capture(transact, time);
					network->EventQueue->Add(happened);
				}
				Table->UpdateState(node, time);
			}

			break;
		case Processed:
			node = network->Channel->Get(transact->GetNode() - 1);
			Table->OnRelease(node, transact, time);
			node->Release();

			Event* event = Event::CreateFreeEvent(transact, time);
			network->EventQueue->Add(event);

			if (node->Queue->Empty() == false)
			{
				Transact* newTransact = node->Queue->Remove();
				Table->OnStart(node, newTransact, time);
				Event* happened = node->Capture(newTransact, time);
				network->EventQueue->Add(happened);
			}
			Table->UpdateState(node, time);
			break;
		}
		delete currentEvent;

	}

	FullStatistic state = Table->ComputeStatistic(network->Terminal);

	//10) Вывод статистики на экран
	Report::Locale* loc = new Report::RussianLocale();
	std::wofstream* file = new std::wofstream("output.txt");
	Report::StreamReport reporter(&std::wcout, loc);
	reporter.Print(state);
	file->close();
	delete file;
	delete loc;

/*	for (int i = 0; i < ammount; i++)
	{
		delete transactList[i];
	}

	delete network->Time;
	delete network->EventQueue;
	delete network->Terminal;
	delete node->Queue;
	delete node->Distribution;
	delete node;
	delete network->Channel;
	delete network->Generator->Distribution;

	
	delete network;*/


}