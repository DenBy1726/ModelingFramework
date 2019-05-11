#include "ModelBuilder.h"
#include "DistributionBuilder.h"
#include "Distribution.cpp"
#include "IDistribution.h"
#include "Node.h"
#include "Model.h"
#include "Generator.h"
#include "StreamReport.cpp"
#include "RussianLocale.cpp"
#include <iostream>
#include <string>
#include <vector>

#include "Util.cpp"

using namespace Study::Struct;
using namespace Study::Work;


namespace Random
{
	class MyDistribution : public IDistribution<double>
	{
		std::vector<double> args;
		int cur_index = 0;

	public:

		MyDistribution(const Random& gen = Random::Random())
		{
			_gen = gen;
		}

		MyDistribution(const std::vector<double>& arg, const Random& gen = Random::Random())
		{
			_gen = gen;
			args = arg;
		}
		double Next()
		{
			cur_index = (cur_index + 1) % args.size();
			return _gen.Next(0, 100) + args[cur_index];
		}

		std::string Type()
		{
			return "MyRandom";
		}


		std::vector<double> Argument()
		{
			return args;
		}

		IDistribution<double>* Clone(const std::vector<double>& arg)
		{
			return new MyDistribution(arg);
		}
	};
}


void HandKarkas()
{
	std::locale::global(std::locale("Rus"));
	//ИНИЦИАЛИЗАЦИЯ//
	//1)создать генератор заявок
	//1.1)создать распределение генератора заявок
	Random::IntExponentialDistribution d({ 0.05 });
	//1.2)создать генератор
	Model::Generator modelGenerator(&d);
	//2)создать очередь событий
	Queue::EventQueue eventQueue;
	//3)создать канал
	//3.1) создать очередь канала
	Queue::FIFOQueue<Model::Transact*> q;
	//3.2) создать генератор задержек для канала
	Random::IntExponentialDistribution dn({ 0.1 });
	//3.3) создать канал
	Model::Node* n = new Model::Node(&dn,1,&q);
	//4) создать матрицу переходов
	BasicMatrix<double> mat(3, 3);
	mat[0][1] = 1;
	mat[1][2] = 1;
	//5)создать модельное время
	Time time = 0;
	//6)создать сеть
	//6.1)создать генератор для определения переходов транзактов
	Random::Random gen;
	//6.2)собрать каналы в список каналов
	std::vector<Model::Node*> nodes;
	nodes.push_back(n);
	//6.2)создать объект сети
	Model::Network net(mat,nodes);
	//6.6)связать сеть с очередью событий
	//6.7)связать сеть с генератором появления заявок

	//6.9)Создать список транзактов
	std::vector<Model::Transact*> transactList;

	//7)загрузить транзакты в сеть
	for (int i = 0; i < 50000; i++)
	{
		//7.1)создать транзакт
		Model::Transact* t = modelGenerator.CreateTransact();
		transactList.push_back(t);
		//7.2)создать событие появления заявки
		Model::Event* e = new Model::Event(Model::Free,t,t->Statistic.Start);
		//7.3)Добавить событие в очередь событий
		eventQueue.Add(e);
	}

	//8) цикл обработки событий)
	//8.1) До тех пор пока все события не обработаны
	while (eventQueue.Empty() == false)
	{
		//8.2)Получить все события которые нужно обработать
		std::vector<Model::Event*> currentEvents = eventQueue.GetNextEvents();
		//8.3)Зафиксировать текущий такт
		time = currentEvents[0]->Time;

		//8.4)Обработать каждое событие
		for each (Model::Event* currentEvent in currentEvents)
		{
			Model::Transact* transact = currentEvent->Sender;
			double probValue, probSum;
			int length, currentNode;

			switch (currentEvent->Id)
			{
				//8.4.1)Если событие типа С1(транзакт готов двигаться)
			case Free:
				//8.4.1.1)Продвигаем транзакт по сети
				transact->Move(&net);
				//8.4.1.2)Если транзакт попал в терминал,помечаем что он обработан
				if (net.IsTerminator(transact->GetNode()))
				{
					transact->OnStop(time);
				}
				//8.4.1.3)Иначе пытаемся занять канал
				else
				{
					currentNode = transact->GetNode() - 1;
					
					//8.4.1.4)Регистрируем транзакт в канале
					net.Channel[currentNode]->Register(transact, time);

					//8.4.1.5)Если канал работает, добавляем транзакт в очередь
					if (net.Channel[currentNode]->IsBusy() == true)
					{
						net.Channel[currentNode]->Queue->Add(0, transact);
					}
					//8.4.1.6)Иначе занимаем канал и добавляем событие в очередь событий
					else
					{
						Model::Event* newEvent = net.Channel[currentNode]->Capture(transact, time);
						eventQueue.Add(newEvent);
					}
					//8.4.1.7)Модифицируем статистику состояний канала
					net.Channel[currentNode]->UpdateState(time);
				}
				break;

				//8.4.2 Если событие типа С2(транзакт закончил обработку)
			case Processed:
				currentNode = transact->GetNode() - 1;

				//8.4.2.1) Освобождаем канал
				net.Channel[currentNode]->OnRelease(transact, time);
				//8.4.2.2)если к каналу есть очередь
				if (net.Channel[currentNode]->Queue->IsEmpty() == false)
				{
					//8.4.2.2.1)Получаем следующий транзакт из очереди к каналу
					Model::Transact* newTransact = net.Channel[currentNode]->Queue->Remove();
					//8.4.1.2.2)Занимаем канал
					Model::Event* newEvent = net.Channel[currentNode]->Capture(newTransact, time);
					eventQueue.Add(newEvent);
				}
				//8.4.2.3)Модифицируем статистику состояний канала
				net.Channel[currentNode]->UpdateState(time);

				//8.4.2.4)теперь транзакт освобожден и его необходимо передвинуть далее.
				//8.4.2.4.1)Создаем событие типа С1(транзактов готов к передвижению)
				Model::Event* newEvent = new Model::Event(Model::Free,transact,time);
				//8.4.2.4.2)Так как транзакт двигать нужно в текущем такте, добавляем событие в список событий
				eventQueue.Add(newEvent);
				break;
			}


			//8.5 Освободить память, занимаемую под событие
			delete currentEvent;
		}
	}

	//9) Расчитать статистику
	Statistic::FullStatistic fullStatistic;
	int nodeCounter = 0;
	//9.1) Статистика каждого канала
	for each (Model::Node* node in net.Channel)
	{
		//9.1.1)создать страницу статистики для текущего канала
		fullStatistic.LocalStatistic.push_back(Statistic::ModelUnit());
		//9.1.2)Расчитать среднюю длинну очереди и загруженность канала из статистики состояний канала
		//средняя длинна очереди
		// = сумма(mi * li) / n
		// li длинна очереди в i состоянии
		// mi длительность i состояния
		// n количество времени
		//загруженность канала ро
		//количество тактов, когда устройство занято / сколько всего тактов
		long double temp = 0;
		double temp2 = 0;
		int lastNotNull = 0;

		std::vector<Statistic::StateStatisticUnit> StateStatistic = node->GetStateStatistic();
		std::vector<Statistic::WorkStatisticUnit> WorkStatistic = node->GetWorkStatistic();

		for (int i = 1; i < StateStatistic.size(); i++)
		{
			if (StateStatistic[i].Time == StateStatistic[i - 1].Time)
				temp += (StateStatistic[i].Time - StateStatistic[lastNotNull].Time) *(StateStatistic[i].QueueLength - StateStatistic[i - 1].QueueLength);
			else
			{
				lastNotNull = i;
				temp += ((StateStatistic[i].Time - StateStatistic[i - 1].Time) * StateStatistic[i - 1].QueueLength);
			}
			//считаем количество тактов, которое канал работает
			if (StateStatistic[i - 1].Busy < node->Channel())
			{
				temp2 += (StateStatistic[i].Time - StateStatistic[i - 1].Time);
			}
		}

		fullStatistic.LocalStatistic[nodeCounter].Length = temp / double(time);
		fullStatistic.LocalStatistic[nodeCounter].Probability = temp2 / double(time);

		//9.1.3)Расчитать среднее время ожидания, среднее время пребывания из статистики работы канала
		//среднее время ожидания w
		// = сумма(такт захвата - такт входа)
		//среднее время пребывани в системе u
		// = сумма(такт освобождения - такт входа
		temp = 0;
		temp2 = 0;
		//it - пара <номер транзакта,Рабочая статистика данного транзакта>
		// Рабочая статистика - такт входа, захвата, освобождения
		for each (auto it in WorkStatistic)
		{
			temp += (it.Start - it.Enter) / double(WorkStatistic.size());
			temp2 += (it.Release - it.Enter) / double(WorkStatistic.size());
		}
		fullStatistic.LocalStatistic[nodeCounter].WaitTime = temp;
		fullStatistic.LocalStatistic[nodeCounter].ProcessingTime = temp2;

		//9.1.4)Рассчитать среднее число заявок в канале как
		//Число заявлк в очереди + загруженность канала
		fullStatistic.LocalStatistic[nodeCounter].TransactAmount =
			fullStatistic.LocalStatistic[nodeCounter].Probability + fullStatistic.LocalStatistic[nodeCounter].Length;


		nodeCounter++;
	}

	//9.2) Расчет коэфиициента передачи каждого канала
	for (int i = 0; i < fullStatistic.LocalStatistic.size(); i++)
	{
		int size = net.Channel[i]->GetWorkStatistic().size();
		fullStatistic.LocalStatistic[i].TransferCoefficient =
			(double)size / (double)transactList.size();
	}

	//9.3) Расчет статистики системы
	for (int i = 0; i < fullStatistic.LocalStatistic.size(); i++)
	{
		fullStatistic.GlobalStatistic.Length += fullStatistic.LocalStatistic[i].Length
			* fullStatistic.LocalStatistic[i].TransferCoefficient;
		fullStatistic.GlobalStatistic.Probability += fullStatistic.LocalStatistic[i].Probability;
		fullStatistic.GlobalStatistic.TransactAmount += fullStatistic.LocalStatistic[i].TransactAmount;
		fullStatistic.GlobalStatistic.ProcessingTime += fullStatistic.LocalStatistic[i].ProcessingTime
			* fullStatistic.LocalStatistic[i].TransferCoefficient;
		fullStatistic.GlobalStatistic.WaitTime += fullStatistic.LocalStatistic[i].WaitTime
			* fullStatistic.LocalStatistic[i].TransferCoefficient;

	}

	for (int i = 0; i < transactList.size(); i++)
	{
		delete transactList[i];
	}

	//10) Вывод статистики на экран
	Report::Locale* loc = new Report::RussianLocale();
	std::wofstream* file = new std::wofstream("output.txt");
	Report::StreamReport reporter(&std::wcout, loc);
	reporter.Print(fullStatistic);
	file->close();
	delete file;
	delete loc;
}

void HandJob()
{
	//ИНИЦИАЛИЗАЦИЯ//
	//1)создать генератор
	Random::IntExponentialDistribution d({ 0.05 });
	//2)создать очередь событий
	Queue::PriorityQueue<Event*> eq;
	//3)создать канал
	//3.1) создать очередь канала
	Queue::FIFOQueue<Study::Work::Transact*> q;
	//3.2) создать генератор задержек для канала
	Random::IntExponentialDistribution dn({ 0.1 });
	//3.3) создать таблицы статистики для канала
	//3.3.1)создать таблицу статистики по смене состояний канала
	std::vector<Statistic::StateStatisticUnit> ststat;
	//3.3.2)создать таблицу статистики по работе канала
	std::vector<Statistic::WorkStatisticUnit> workstat;
	//3.4) создать канал
	Node n;
	//3.5) связать канал
	//3.5.1) с очередью канала
	n.Queue = q;
	//3.5.2) с генератором задержек
	n.Generator = &dn;
	//3.5.3) с таблицей статистики
	n.StateStatistic = ststat;
	n.WorkStatistic = workstat;
	//4) создать матрицу переходов
	Matrix<double> mat(3, 3);
	mat[0][1] = 1;
	mat[1][2] = 1;
	//5)создать модельное время
	Time modtime = 0;
	//6)создать сеть
	//6.1)создать генератор для определения переходов транзактов
	Random::Random gen;
	//6.2)создать объект сети
	Network net;
	//6.3)добавить в сеть узлы
	net.Nodes.push_back(n);
	//6.4)связать сеть с матрицей
	net.Matrix = mat;
	//6.5)связать сеть с временем
	net.Time = modtime;
	//6.6)связать сеть с очередью событий
	net.Queue = eq;
	//6.7)связать сеть с генератором появления заявок
	net.EnterGenerator = &d;
	//6.8)связать генератор с генератором переходов транзактов
	net.Generator = gen;

	//6.9)Создать список транзактов
	std::vector<Transact*> transactList;

	int elapsed = 0;
	int summ = 0;
	//7)загрузить транзакты в сеть
	for (int i = 0; i < 50000; i++)
	{
		//7.1)создать транзакт
		Study::Work::Transact* t = new Study::Work::Transact();
		transactList.push_back(t);
		t->Id = i;
		//7.2)сгенерировать время в которое заявка появится
		elapsed = net.EnterGenerator->Next();
		summ += elapsed;
		t->Statistic.Start = summ;
		t->CurrentNode = 0;
		//7.3)создать событие появления заявки
		Event* e = new Event();
		e->Id = Free;
		e->Time = summ;
		e->Sender = t;
		//7.4)Добавить событие в очередь событий
		net.Queue.Add(e->Time,e);
	}
	
	//8) цикл обработки событий
	//8.1) До тех пор пока все события не обработаны
	while (net.Queue.IsEmpty() == false)
	{
		//8.2)Зафиксировать текущий такт
		net.Time = net.Queue.Top()->Time;
		//8.3)Получить все события в текущий такт
		std::vector<Event*> currentEvents;
		do
		{
			currentEvents.push_back(net.Queue.Remove());
		}
		while (net.Queue.IsEmpty() == false && net.Queue.Top()->Time == net.Time);
		//8.4)Обработать каждое событие
		for each (Event* currentEvent in currentEvents)
		{
			Transact* transact = currentEvent->Sender;
			double probValue, probSum;
			int length, currentNode;

			switch (currentEvent->Id)
			{
				//8.4.1)Если событие типа С1(транзакт готов двигаться)
			case Free:
				//8.4.1.1)Продвигаем транзакт по сети
				//8.4.1.1.1)Генерируем число для определения нового узла
				probValue = net.Generator.NextDouble();
				//8.4.1.1.2)Получаем количество узлов в матрице
				length = net.Matrix.height();
				probSum = 0;
				//8.4.1.1.3)Определяем на основании сгенерированного числа и матрицы переходов в какой
				//узел перейдет транзакт. Цикл начинается с 1 в предположении что на генератор перейти нельзя
				for (int i = 1; i < length; i++)
				{
					probSum += net.Matrix[transact->CurrentNode][i];
					if (probSum > probValue)
					{
						transact->CurrentNode = i;
						break;
					}
				}
				//8.4.1.2)Если транзакт попал в терминал,помечаем что он обработан
				if (transact->CurrentNode > net.Nodes.size())
				{
					transact->Statistic.Finish = net.Time;
				}
				//8.4.1.3)Иначе пытаемся занять канал
				else
				{
					currentNode = transact->CurrentNode - 1;
					//Event* e = network->Channel[newNode - 1]->BeginCapture(transact);
					//8.4.1.4)Регистрируем транзакт в канале
					//8.4.1.4.1)Добавляем в статистку канала запись о том, что транзакт вошел в канал
					net.Nodes[currentNode].WorkStatistic.push_back(Statistic::WorkStatisticUnit());
					net.Nodes[currentNode].WorkStatistic.back().Enter = net.Time;
					//8.4.1.4.2)Запоминаем в транзакте номер статистики,с которой он связан
					transact->Cell = net.Nodes[currentNode].WorkStatistic.size() - 1;
					//8.4.1.5)Если канал работает, добавляем транзакт в очередь
					if (net.Nodes[currentNode].IsWorking == true)
					{
						net.Nodes[currentNode].Queue.Add(0, transact);
					}
					//8.4.1.6)Иначе занимаем канал
					else
					{
						//8.4.1.6.1)Помечаем в статистике что транзакт занял канал
						net.Nodes[currentNode].WorkStatistic[transact->Cell].Start = net.Time;
						//8.4.1.6.2)Устанавливаем флаг занятости канала в состояния Занят
						net.Nodes[currentNode].IsWorking = true;
						//8.4.1.6.3)Расчитываем время окончания обработки транзакта
						int timeToProcessed = net.Time + net.Nodes[currentNode].Generator->Next();
						//8.4.1.6.4)Создаем событие окончания обработки транзакта(C2)
						Event* newEvent = new Event();
						newEvent->Id = EventId::Processed;
						newEvent->Sender = transact;
						newEvent->Time = timeToProcessed;
						//8.4.1.6.5)Добавляем созданное событие в очередь событий
						net.Queue.Add(newEvent->Time, newEvent);
					}
					//8.4.1.7)Модифицируем статистику состояний канала
					//8.4.1.7.1)Создаем новую статистику страницы и заполняем ее
					Statistic::StateStatisticUnit temp;
					temp.Busy = net.Nodes[currentNode].IsWorking;
					temp.Time = net.Time;
					temp.QueueLength = net.Nodes[currentNode].Queue.Length();
					//8.4.1.7.2)Добавляем страницу в статистику
					net.Nodes[currentNode].StateStatistic.push_back(temp);
				}
				break;

				//8.4.2 Если событие типа С2(транзакт закончил обработку)
			case Processed:
				currentNode = transact->CurrentNode - 1;

				//8.4.2.1) Освобождаем канал
				//8.4.2.1.1)Отмечаем в статистике что канал освободился
				net.Nodes[currentNode].WorkStatistic[transact->Cell].Release = net.Time;
				//8.4.2.1.2) Переключаем флаг занятости в состояние не занят
				net.Nodes[currentNode].IsWorking = false;
				//8.4.2.1.3) сбрасываем в транзакте номер статистики,с которой он связан
				transact->Cell = -1;
				//8.4.2.2)если к каналу есть очередь
				if (net.Nodes[currentNode].Queue.IsEmpty() == false)
				{
					//8.4.2.2.1)Получаем следующий транзакт из очереди к каналу
					Transact* newTransact = net.Nodes[currentNode].Queue.Remove();
					//8.4.2.2.2)Занимаем канал
					//8.4.2.2.2.1)Помечаем в статистике что транзакт занял канал
					net.Nodes[currentNode].WorkStatistic[newTransact->Cell].Start = net.Time;
					//8.4.2.2.2.2)Устанавливаем флаг занятости канала в состояния Занят
					net.Nodes[currentNode].IsWorking = true;
					//8.4.2.2.2.3)Расчитываем время окончания обработки транзакта
					int timeToProcessed = net.Time + net.Nodes[currentNode].Generator->Next();
					//8.4.2.2.2.4)Создаем событие окончания обработки транзакта(C2)
					Event* newEvent = new Event();
					newEvent->Id = EventId::Processed;
					newEvent->Sender = newTransact;
					newEvent->Time = timeToProcessed;
					//8.4.2.2.2.5)Добавляем созданное событие в очередь событий
					net.Queue.Add(newEvent->Time, newEvent);
				}
				//8.4.2.3)Модифицируем статистику состояний канала
				//8.4.2.3.1)Создаем новую статистику страницы и заполняем ее
				Statistic::StateStatisticUnit temp;
				temp.Busy = net.Nodes[currentNode].IsWorking;
				temp.Time = net.Time;
				temp.QueueLength = net.Nodes[currentNode].Queue.Length();
				//8.4.2.3.2)Добавляем страницу в статистику
				net.Nodes[currentNode].StateStatistic.push_back(temp);

				//8.4.2.4)теперь транзакт освобожден и его необходимо передвинуть далее.
				//8.4.2.4.1)Создаем событие типа С1(транзактов готов к передвижению)
				Event* newEvent = new Event();
				newEvent->Id = EventId::Free;
				newEvent->Time = net.Time;
				newEvent->Sender = transact;
				//8.4.2.4.2)Так как транзакт двигать нужно в текущем такте, добавляем событие в список событий
				net.Queue.Add(newEvent->Time, newEvent);
				break;
			}


			//8.5 Освободить память, занимаемую под событие
			delete currentEvent;
		}
	}

	//9) Расчитать статистику
	Statistic::FullStatistic fullStatistic;
	int nodeCounter = 0;
	//9.1) Статистика каждого канала
	for each (Node node in net.Nodes)
	{
		//9.1.1)создать страницу статистики для текущего канала
		fullStatistic.LocalStatistic.push_back(Statistic::ModelUnit());
		//9.1.2)Расчитать среднюю длинну очереди и загруженность канала из статистики состояний канала
		//средняя длинна очереди
		// = сумма(mi * li) / n
		// li длинна очереди в i состоянии
		// mi длительность i состояния
		// n количество времени
		//загруженность канала ро
		//количество тактов, когда устройство занято / сколько всего тактов
		long double temp = 0;
		double temp2 = 0;
		int lastNotNull = 0;

		for (int i = 1; i < node.StateStatistic.size(); i++)
		{
			if (node.StateStatistic[i].Time == node.StateStatistic[i - 1].Time)
				temp += (node.StateStatistic[i].Time - node.StateStatistic[lastNotNull].Time) *(node.StateStatistic[i].QueueLength - node.StateStatistic[i - 1].QueueLength);
			else
			{
				lastNotNull = i;
				temp += ((node.StateStatistic[i].Time - node.StateStatistic[i - 1].Time) * node.StateStatistic[i - 1].QueueLength);
			}
			//считаем количество тактов, которое канал работает
			if (node.StateStatistic[i - 1].Busy == true)
			{
				temp2 += (node.StateStatistic[i].Time - node.StateStatistic[i - 1].Time);
			}
		}

		fullStatistic.LocalStatistic[nodeCounter].Length = temp / double(net.Time);
		fullStatistic.LocalStatistic[nodeCounter].Probability = temp2 / double(net.Time);

		//9.1.3)Расчитать среднее время ожидания, среднее время пребывания из статистики работы канала
		//среднее время ожидания w
		// = сумма(такт захвата - такт входа)
		//среднее время пребывани в системе u
		// = сумма(такт освобождения - такт входа
		temp = 0;
		temp2 = 0;
		//it - пара <номер транзакта,Рабочая статистика данного транзакта>
		// Рабочая статистика - такт входа, захвата, освобождения
		for each (auto it in node.WorkStatistic)
		{
			temp += (it.Start - it.Enter) / double(node.WorkStatistic.size());
			temp2 += (it.Release - it.Enter) / double(node.WorkStatistic.size());
		}
		fullStatistic.LocalStatistic[nodeCounter].WaitTime = temp;
		fullStatistic.LocalStatistic[nodeCounter].ProcessingTime = temp2;

		//9.1.4)Рассчитать среднее число заявок в канале как
		//Число заявлк в очереди + загруженность канала
		fullStatistic.LocalStatistic[nodeCounter].TransactAmount =
			fullStatistic.LocalStatistic[nodeCounter].Probability + fullStatistic.LocalStatistic[nodeCounter].Length;


		nodeCounter++;
	}

	//9.2) Расчет коэфиициента передачи каждого канала
	for (int i = 0; i < fullStatistic.LocalStatistic.size(); i++)
	{
		fullStatistic.LocalStatistic[i].TransferCoefficient = 
			(double)net.Nodes[i].WorkStatistic.size() / (double)transactList.size();
	}

	//9.3) Расчет статистики системы
	for (int i = 0; i < fullStatistic.LocalStatistic.size(); i++)
	{
		fullStatistic.GlobalStatistic.Length += fullStatistic.LocalStatistic[i].Length
			* fullStatistic.LocalStatistic[i].TransferCoefficient;
		fullStatistic.GlobalStatistic.Probability += fullStatistic.LocalStatistic[i].Probability;
		fullStatistic.GlobalStatistic.TransactAmount += fullStatistic.LocalStatistic[i].TransactAmount;
		fullStatistic.GlobalStatistic.ProcessingTime += fullStatistic.LocalStatistic[i].ProcessingTime
			* fullStatistic.LocalStatistic[i].TransferCoefficient;
		fullStatistic.GlobalStatistic.WaitTime += fullStatistic.LocalStatistic[i].WaitTime
			* fullStatistic.LocalStatistic[i].TransferCoefficient;

	}

	//10) Вывод статистики на экран
	Report::Locale* loc = new Report::RussianLocale();
	//std::wofstream* file = new std::wofstream("input.txt");
	Report::StreamReport reporter(&std::wcout, loc);
	reporter.Print(fullStatistic);
	
	
	
	

}
	
void HandJob2()
{
	//ИНИЦИАЛИЗАЦИЯ//
	//1)создать генератор
	Random::IntExponentialDistribution d({ 0.05 });
	//2)создать очередь событий
	Queue::PriorityQueue<Event*> eq;
	//3)создать канал
	//3.1) создать очередь канала
	Queue::FIFOQueue<Study::Work::Transact*> q1;
	Queue::FIFOQueue<Study::Work::Transact*> q2;
	Queue::FIFOQueue<Study::Work::Transact*> q3;
	//3.2) создать генератор задержек для канала
	Random::IntExponentialDistribution dn1({ 0.25 });
	Random::IntExponentialDistribution dn2({ 0.33 });
	Random::IntExponentialDistribution dn3({ 0.1 });
	//3.3) создать таблицы статистики для канала
	//3.3.1)создать таблицу статистики по смене состояний канала
	std::vector<Statistic::StateStatisticUnit> ststat1;
	std::vector<Statistic::StateStatisticUnit> ststat2;
	std::vector<Statistic::StateStatisticUnit> ststat3;
	//3.3.2)создать таблицу статистики по работе канала
	std::vector<Statistic::WorkStatisticUnit> workstat1;
	std::vector<Statistic::WorkStatisticUnit> workstat2;
	std::vector<Statistic::WorkStatisticUnit> workstat3;
	//3.4) создать канал
	Node n1;
	Node n2;
	Node n3;
	//3.5) связать канал
	//3.5.1) с очередью канала
	n1.Queue = q1;
	n2.Queue = q2;
	n3.Queue = q3;
	//3.5.2) с генератором задержек
	n1.Generator = &dn1;
	n2.Generator = &dn2;
	n3.Generator = &dn3;
	//3.5.3) с таблицей статистики
	n1.StateStatistic = ststat1;
	n2.StateStatistic = ststat2;
	n3.StateStatistic = ststat3;
	n1.WorkStatistic = workstat1;
	n2.WorkStatistic = workstat2;
	n3.WorkStatistic = workstat3;
	//4) создать матрицу переходов
	Matrix<double> mat(5, 5);
	mat[0][1] = 1;
	mat[1][1] = 0.6;
	mat[1][2] = 0.16;
	mat[1][3] = 0.144;
	mat[1][4] = 0.096;
	mat[2][2] = 0.3;
	mat[2][3] = 0.7;
	mat[3][4] = 1;
	//5)создать модельное время
	Time modtime = 0;
	//6)создать сеть
	//6.1)создать генератор для определения переходов транзактов
	Random::Random gen;
	//6.2)создать объект сети
	Network net;
	//6.3)добавить в сеть узлы
	net.Nodes.push_back(n1);
	net.Nodes.push_back(n2);
	net.Nodes.push_back(n3);
	//6.4)связать сеть с матрицей
	net.Matrix = mat;
	//6.5)связать сеть с временем
	net.Time = modtime;
	//6.6)связать сеть с очередью событий
	net.Queue = eq;
	//6.7)связать сеть с генератором появления заявок
	net.EnterGenerator = &d;
	//6.8)связать генератор с генератором переходов транзактов
	net.Generator = gen;

	//6.9)Создать список транзактов
	std::vector<Transact*> transactList;

	int elapsed = 0;
	int summ = 0;
	//7)загрузить транзакты в сеть
	for (int i = 0; i < 50000; i++)
	{
		//7.1)создать транзакт
		Study::Work::Transact* t = new Study::Work::Transact();
		transactList.push_back(t);
		t->Id = i;
		//7.2)сгенерировать время в которое заявка появится
		elapsed = net.EnterGenerator->Next();
		summ += elapsed;
		t->Statistic.Start = summ;
		t->CurrentNode = 0;
		//7.3)создать событие появления заявки
		Event* e = new Event();
		e->Id = Free;
		e->Time = summ;
		e->Sender = t;
		//7.4)Добавить событие в очередь событий
		net.Queue.Add(e->Time, e);
	}

	//8) цикл обработки событий
	//8.1) До тех пор пока все события не обработаны
	while (net.Queue.IsEmpty() == false)
	{
		//8.2)Зафиксировать текущий такт
		net.Time = net.Queue.Top()->Time;
		//8.3)Получить все события в текущий такт
		std::vector<Event*> currentEvents;
		do
		{
			currentEvents.push_back(net.Queue.Remove());
		}
		while (net.Queue.IsEmpty() == false && net.Queue.Top()->Time == net.Time);
		//8.4)Обработать каждое событие
		for each (Event* currentEvent in currentEvents)
		{
			Transact* transact = currentEvent->Sender;
			double probValue, probSum;
			int length, currentNode;

			switch (currentEvent->Id)
			{
				//8.4.1)Если событие типа С1(транзакт готов двигаться)
			case Free:
				//8.4.1.1)Продвигаем транзакт по сети
				//8.4.1.1.1)Генерируем число для определения нового узла
				probValue = net.Generator.NextDouble();
				//8.4.1.1.2)Получаем количество узлов в матрице
				length = net.Matrix.height();
				probSum = 0;
				//8.4.1.1.3)Определяем на основании сгенерированного числа и матрицы переходов в какой
				//узел перейдет транзакт. Цикл начинается с 1 в предположении что на генератор перейти нельзя
				for (int i = 1; i < length; i++)
				{
					probSum += net.Matrix[transact->CurrentNode][i];
					if (probSum > probValue)
					{
						transact->CurrentNode = i;
						break;
					}
				}
				//8.4.1.2)Если транзакт попал в терминал,помечаем что он обработан
				if (transact->CurrentNode > net.Nodes.size())
				{
					transact->Statistic.Finish = net.Time;
				}
				//8.4.1.3)Иначе пытаемся занять канал
				else
				{
					currentNode = transact->CurrentNode - 1;
					//Event* e = network->Channel[newNode - 1]->BeginCapture(transact);
					//8.4.1.4)Регистрируем транзакт в канале
					//8.4.1.4.1)Добавляем в статистку канала запись о том, что транзакт вошел в канал
					net.Nodes[currentNode].WorkStatistic.push_back(Statistic::WorkStatisticUnit());
					net.Nodes[currentNode].WorkStatistic.back().Enter = net.Time;
					//8.4.1.4.2)Запоминаем в транзакте номер статистики,с которой он связан
					transact->Cell = net.Nodes[currentNode].WorkStatistic.size() - 1;
					//8.4.1.5)Если канал работает, добавляем транзакт в очередь
					if (net.Nodes[currentNode].IsWorking == true)
					{
						net.Nodes[currentNode].Queue.Add(0, transact);
					}
					//8.4.1.6)Иначе занимаем канал
					else
					{
						//8.4.1.6.1)Помечаем в статистике что транзакт занял канал
						net.Nodes[currentNode].WorkStatistic[transact->Cell].Start = net.Time;
						//8.4.1.6.2)Устанавливаем флаг занятости канала в состояния Занят
						net.Nodes[currentNode].IsWorking = true;
						//8.4.1.6.3)Расчитываем время окончания обработки транзакта
						int timeToProcessed = net.Time + net.Nodes[currentNode].Generator->Next();
						//8.4.1.6.4)Создаем событие окончания обработки транзакта(C2)
						Event* newEvent = new Event();
						newEvent->Id = EventId::Processed;
						newEvent->Sender = transact;
						newEvent->Time = timeToProcessed;
						//8.4.1.6.5)Добавляем созданное событие в очередь событий
						net.Queue.Add(newEvent->Time, newEvent);
					}
					//8.4.1.7)Модифицируем статистику состояний канала
					//8.4.1.7.1)Создаем новую статистику страницы и заполняем ее
					Statistic::StateStatisticUnit temp;
					temp.Busy = net.Nodes[currentNode].IsWorking;
					temp.Time = net.Time;
					temp.QueueLength = net.Nodes[currentNode].Queue.Length();
					//8.4.1.7.2)Добавляем страницу в статистику
					net.Nodes[currentNode].StateStatistic.push_back(temp);
				}
				break;

				//8.4.2 Если событие типа С2(транзакт закончил обработку)
			case Processed:
				currentNode = transact->CurrentNode - 1;

				//8.4.2.1) Освобождаем канал
				//8.4.2.1.1)Отмечаем в статистике что канал освободился
				net.Nodes[currentNode].WorkStatistic[transact->Cell].Release = net.Time;
				//8.4.2.1.2) Переключаем флаг занятости в состояние не занят
				net.Nodes[currentNode].IsWorking = false;
				//8.4.2.1.3) сбрасываем в транзакте номер статистики,с которой он связан
				transact->Cell = -1;
				//8.4.2.2)если к каналу есть очередь
				if (net.Nodes[currentNode].Queue.IsEmpty() == false)
				{
					//8.4.2.2.1)Получаем следующий транзакт из очереди к каналу
					Transact* newTransact = net.Nodes[currentNode].Queue.Remove();
					//8.4.2.2.2)Занимаем канал
					//8.4.2.2.2.1)Помечаем в статистике что транзакт занял канал
					net.Nodes[currentNode].WorkStatistic[newTransact->Cell].Start = net.Time;
					//8.4.2.2.2.2)Устанавливаем флаг занятости канала в состояния Занят
					net.Nodes[currentNode].IsWorking = true;
					//8.4.2.2.2.3)Расчитываем время окончания обработки транзакта
					int timeToProcessed = net.Time + net.Nodes[currentNode].Generator->Next();
					//8.4.2.2.2.4)Создаем событие окончания обработки транзакта(C2)
					Event* newEvent = new Event();
					newEvent->Id = EventId::Processed;
					newEvent->Sender = newTransact;
					newEvent->Time = timeToProcessed;
					//8.4.2.2.2.5)Добавляем созданное событие в очередь событий
					net.Queue.Add(newEvent->Time, newEvent);
				}
				//8.4.2.3)Модифицируем статистику состояний канала
				//8.4.2.3.1)Создаем новую статистику страницы и заполняем ее
				Statistic::StateStatisticUnit temp;
				temp.Busy = net.Nodes[currentNode].IsWorking;
				temp.Time = net.Time;
				temp.QueueLength = net.Nodes[currentNode].Queue.Length();
				//8.4.2.3.2)Добавляем страницу в статистику
				net.Nodes[currentNode].StateStatistic.push_back(temp);

				//8.4.2.4)теперь транзакт освобожден и его необходимо передвинуть далее.
				//8.4.2.4.1)Создаем событие типа С1(транзактов готов к передвижению)
				Event* newEvent = new Event();
				newEvent->Id = EventId::Free;
				newEvent->Time = net.Time;
				newEvent->Sender = transact;
				//8.4.2.4.2)Так как транзакт двигать нужно в текущем такте, добавляем событие в список событий
				net.Queue.Add(newEvent->Time, newEvent);
				break;
			}


			//8.5 Освободить память, занимаемую под событие
			delete currentEvent;
		}
	}

	//9) Расчитать статистику
	Statistic::FullStatistic fullStatistic;
	int nodeCounter = 0;
	//9.1) Статистика каждого канала
	for each (Node node in net.Nodes)
	{
		//9.1.1)создать страницу статистики для текущего канала
		fullStatistic.LocalStatistic.push_back(Statistic::ModelUnit());
		//9.1.2)Расчитать среднюю длинну очереди и загруженность канала из статистики состояний канала
		//средняя длинна очереди
		// = сумма(mi * li) / n
		// li длинна очереди в i состоянии
		// mi длительность i состояния
		// n количество времени
		//загруженность канала ро
		//количество тактов, когда устройство занято / сколько всего тактов
		long double temp = 0;
		double temp2 = 0;
		int lastNotNull = 0;

		for (int i = 1; i < node.StateStatistic.size(); i++)
		{
			if (node.StateStatistic[i].Time == node.StateStatistic[i - 1].Time)
				temp += (node.StateStatistic[i].Time - node.StateStatistic[lastNotNull].Time) *(node.StateStatistic[i].QueueLength - node.StateStatistic[i - 1].QueueLength);
			else
			{
				lastNotNull = i;
				temp += ((node.StateStatistic[i].Time - node.StateStatistic[i - 1].Time) * node.StateStatistic[i - 1].QueueLength);
			}
			//считаем количество тактов, которое канал работает
			if (node.StateStatistic[i - 1].Busy == true)
			{
				temp2 += (node.StateStatistic[i].Time - node.StateStatistic[i - 1].Time);
			}
		}

		fullStatistic.LocalStatistic[nodeCounter].Length = temp / double(net.Time);
		fullStatistic.LocalStatistic[nodeCounter].Probability = temp2 / double(net.Time);

		//9.1.3)Расчитать среднее время ожидания, среднее время пребывания из статистики работы канала
		//среднее время ожидания w
		// = сумма(такт захвата - такт входа)
		//среднее время пребывани в системе u
		// = сумма(такт освобождения - такт входа
		temp = 0;
		temp2 = 0;
		//it - пара <номер транзакта,Рабочая статистика данного транзакта>
		// Рабочая статистика - такт входа, захвата, освобождения
		for each (auto it in node.WorkStatistic)
		{
			temp += (it.Start - it.Enter) / double(node.WorkStatistic.size());
			temp2 += (it.Release - it.Enter) / double(node.WorkStatistic.size());
		}
		fullStatistic.LocalStatistic[nodeCounter].WaitTime = temp;
		fullStatistic.LocalStatistic[nodeCounter].ProcessingTime = temp2;

		//9.1.4)Рассчитать среднее число заявок в канале как
		//Число заявлк в очереди + загруженность канала
		fullStatistic.LocalStatistic[nodeCounter].TransactAmount =
			fullStatistic.LocalStatistic[nodeCounter].Probability + fullStatistic.LocalStatistic[nodeCounter].Length;


		nodeCounter++;
	}

	//9.2) Расчет коэфиициента передачи каждого канала
	for (int i = 0; i < fullStatistic.LocalStatistic.size(); i++)
	{
		fullStatistic.LocalStatistic[i].TransferCoefficient =
			(double)net.Nodes[i].WorkStatistic.size() / (double)transactList.size();
	}

	//9.3) Расчет статистики системы
	for (int i = 0; i < fullStatistic.LocalStatistic.size(); i++)
	{
		fullStatistic.GlobalStatistic.Length += fullStatistic.LocalStatistic[i].Length
			* fullStatistic.LocalStatistic[i].TransferCoefficient;
		fullStatistic.GlobalStatistic.Probability += fullStatistic.LocalStatistic[i].Probability;
		fullStatistic.GlobalStatistic.TransactAmount += fullStatistic.LocalStatistic[i].TransactAmount;
		fullStatistic.GlobalStatistic.ProcessingTime += fullStatistic.LocalStatistic[i].ProcessingTime
			* fullStatistic.LocalStatistic[i].TransferCoefficient;
		fullStatistic.GlobalStatistic.WaitTime += fullStatistic.LocalStatistic[i].WaitTime
			* fullStatistic.LocalStatistic[i].TransferCoefficient;

	}

	//10) Вывод статистики на экран
	Report::Locale* loc = new Report::RussianLocale();
	//std::wofstream* file = new std::wofstream("input.txt");
	Report::StreamReport reporter(&std::wcout, loc);
	reporter.Print(fullStatistic);





}

void MyDistributionUse()
{
	
	Random::DistributionBuilder::Distribution["MyDistribution"] = new Random::MyDistribution();
	

	BasicMatrix<double> tr(3);
	tr[0][1] = 1;
	tr[1][2] = 1;
	std::vector<Model::Node*> ngen;
	ngen.push_back(new Model::Node(Random::Distribution::Create("MyDistribution", { 1, 2, 3, 4 }),1));
	Random::IDistribution<double>* modelgen = Random::Distribution::Create("IntExponentialDistribution", { 1.0 / 20.0 });

	Model::Model m(tr, ngen, modelgen, Queue::Heap23);

	m.Start(1000);
	auto s = m.ComputeStatistic();

	//std::ofstream file2("model2.txt");
	//	file2 << mb.Save(m);

	Report::Locale* loc = new Report::RussianLocale();
	//std::wofstream* file = new std::wofstream("input.txt");
	Report::StreamReport reporter(&std::wcout, loc);
	reporter.Print(s);

}

void SerializationWork()
{
	//локаль для вывода русского текста в файл
	std::locale::global(std::locale("Rus"));

	//Добавление в сериализатор уже описанного генератора
	Model::ModelBuilder mb;
	mb.Command["NORMAL"] = "NormalDistribution";

	//создаем матрицу
	BasicMatrix<double> bm(3, 3);
	bm[0][1] = 1;
	bm[1][2] = 1;

	//создаем узел
	std::vector<Model::Node*> vd;
	vd.push_back(new Model::Node(Random::Distribution::Create("NormalDistribution", { 5.0,2 }), 1));

	//создаем генератор заявок
	Random::IDistribution<double>* modelgen = Random::Distribution::Create("IntExponentialDistribution", { 1.0 / 20.0 });

	//создаем модель
	Model::Model m(bm, vd, modelgen, Queue::Heap23);

	//запускаем модель
	m.Start(1000);

	//считаем статистику
	auto s = m.ComputeStatistic();

	//печать отчета
	Report::Locale* loc = new Report::RussianLocale();
	std::wofstream* file = new std::wofstream("output.txt");
	Report::StreamReport reporter(file, loc);
	reporter.Print(s);

	file->close();
	delete file;
	delete loc;

	//сохранение модели в файл
	std::string text = mb.Save(m);
    std::ofstream file2("model_normal.txt");
	file2 << mb.Save(m);

	file2.close();

}

void DeserializationWork()
{
	//локаль для вывода русского текста в файл
	std::locale::global(std::locale("Rus"));

	//загрузка текста из файла
	std::string path = "Test\\model_normal.txt";
	std::ifstream mfile(path);
	std::string modelText;
	char buff;

	for (; mfile;)
	{
		mfile.get(buff);
		if (mfile.eof()) break;
		modelText += buff;
	}

	mfile.close();

	//Добавление в сериализатор уже описанного генератора
	Model::ModelBuilder mb;
	mb.Command["NORMAL"] = "NormalDistribution";

	Model::Model m = *mb.Load(modelText, Queue::Heap23);

	//запускаем модель
	m.Start(1000);

	//считаем статистику
	auto s = m.ComputeStatistic();

	//печать отчета
	Report::Locale* loc = new Report::RussianLocale();
	std::wofstream* file = new std::wofstream("output.txt");
	Report::StreamReport reporter(file, loc);
	reporter.Print(s);

	file->close();
	delete file;
	delete loc;




}

void TransactTest()
{
	BasicMatrix<double> bm(5, 5);
	bm[0][1] = 0.5;
	bm[0][2] = 0.5;
	bm[1][4] = 1;
	bm[2][3] = 1;
	bm[3][1] = 0.7;
	bm[3][4] = 0.3;
	std::vector<Model::Node* > vd;
	vd.push_back(new Model::Node(Random::Distribution::Create("IntExponentialDistribution", { 5.0 }), 1));
	vd.push_back(new Model::Node(Random::Distribution::Create("IntExponentialDistribution", { 3.0 }), 1));
	vd.push_back(new Model::Node(Random::Distribution::Create("IntExponentialDistribution", { 7.0 }), 1));
	vd.push_back(new Model::Node(Random::Distribution::Create("IntExponentialDistribution", { 11.0 }), 1));
	Random::IRandom* r = new Random::Random();

	Model::Network n(bm, vd);
	Model::Transact t, t1, t2, t3;
	int a = t.GetAmount();
	t3.GetId();
	t2.GetNode();
	t2.Move(&n);

	Model::Node* d = new Model::Node(Random::Distribution::Create("IntExponentialDistribution", { 2.0 }), 1);
	auto e1 = d->TryCapture(&t, 122);
	auto e2 = d->TryCapture(&t1, 122);
	auto e3 = d->TryCapture(&t2, 122);
	auto e4 = d->TryCapture(&t3, 122);



}

void GoTest(std::vector<int> numbers)
{
	for (int i = 0; i < numbers.size(); i++)
	{
		std::string path = std::string("Test\\") + std::to_string(numbers[i]) + ".txt";
		std::ifstream file(path);
		std::string modelText;
		char buff;

		for (; file;)
		{
			file.get(buff);
			if (file.eof()) break;
			modelText += buff;
		}

		file.close();

		Model::ModelBuilder builder;
		Model::Model m = *builder.Load(modelText);
		

		m.Start(50000);
		auto Stat = m.ComputeStatistic();

		Report::Locale* loc = new Report::RussianLocale();
		//std::wofstream* file = new std::wofstream("input.txt");
		Report::StreamReport reporter(&std::wcout, loc);
		reporter.Name = L"Report " + std::to_wstring(numbers[i]);
		reporter.Print(Stat);

		delete loc;

		getchar();

		
	}
}