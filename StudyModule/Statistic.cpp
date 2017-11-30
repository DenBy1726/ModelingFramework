#include "Statistic.h"
namespace Statistic
{
	StatisticTable::StatisticTable(int ammountOF)
	{
		StateTable.resize(ammountOF);
		WorkTable.resize(ammountOF);
	}

	void StatisticTable::Register(Model::Node* node, Model::Transact* haveEntered, const int& time)
	{
		OnEnter(node, time);
		haveEntered->Cell = WorkTable[node->GetId()].size() - 1;
	}

	void StatisticTable::OnEnter(Model::Node* node, const int& time)
	{
		WorkTable[node->GetId()].push_back(Statistic::WorkStatisticUnit());
		WorkTable[node->GetId()].back().Enter = time;
	}

	//Вызывется когда транзакт начинает обработку
	void StatisticTable::OnStart(Model::Node* node, Model::Transact* tr, const int& time)
	{
		WorkTable[node->GetId()][tr->Cell].Start = time;
	}
	//Вызывется когда транзакт завершает обработку
	void StatisticTable::OnRelease(Model::Node* node, Model::Transact* tr, const int& time)
	{
		WorkTable[node->GetId()][tr->Cell].Release = time;
		tr->Cell = -1;
	}

	//вызывается всякий раз когда изменяется состояние канала или очереди
	void StatisticTable::UpdateState(Model::Node* node, const int& time)
	{
		Statistic::StateStatisticUnit temp;
		temp.Busy = node->Channel() - node->Capacity();
		temp.Time = time;
		temp.QueueLength = node->Queue->Length();
		StateTable[node->GetId()].push_back(temp);
	}

	FullStatistic StatisticTable::ComputeStatistic(Model::Terminal* terminal)
	{
		int currentTime = StateTable.back().back().Time;
		Statistic::FullStatistic state;
		state.ModelInfo.ModelTime = currentTime;
		state.ModelInfo.TransactAmount = terminal->GetCount();
		int nodeCounter = 0;
		double probabilitySum = 0;

		for each (std::vector<StateStatisticUnit> node in StateTable)
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

			for (int i = 1; i < node.size(); i++)
			{
				//после каждой компоненты делим на время чтобы снизить вероятность переполнения типа
				if (node[i].Time == node[i - 1].Time)
				{
					temp += (node[i].Time - node[lastNotNull].Time) *(node[i].QueueLength - node[i - 1].QueueLength);
					temp3 += (node[i].Time - node[lastNotNull].Time) *(node[i].QueueLength - node[i - 1].QueueLength)
						+ (node[i].Time - node[lastNotNull].Time) * node[i - 1].Busy;
				}
				else
				{
					lastNotNull = i;
					temp += ((node[i].Time - node[i - 1].Time) * node[i - 1].QueueLength);
					temp3 += ((node[i].Time - node[i - 1].Time) * node[i - 1].QueueLength)
						+ (node[i].Time - node[i - 1].Time) * node[i - 1].Busy;
				}
				//считаем количество тактов, которое канал работает
				if (node[i - 1].Busy > 0)
				{
					//время,которое канал находился в одном состоянии * процент загрузки канала
					temp2 += (node[i].Time - node[i - 1].Time)*((double)node[i - 1].Busy / (double)_channel[nodeCounter]);
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
		
			//	state.LocalStatistic[nodeCounter].TransactAmount =
			//		state.LocalStatistic[nodeCounter].Probability + state.LocalStatistic[nodeCounter].Length;

			//state.LocalStatistic[nodeCounter].WaitTime / state.LocalStatistic[nodeCounter].ProcessingTime;

			nodeCounter++;
		}

		nodeCounter = 0;
		double temp = 0,temp2 = 0;
		for each (std::vector<WorkStatisticUnit> node in WorkTable)
		{
			for (int i = 0; i < node.size(); i++)
			{

				temp += (node[i].Start - node[i].Enter) / double(node.size());
				temp2 += (node[i].Release - node[i].Enter) / double(node.size());
			}
			state.LocalStatistic[nodeCounter].WaitTime = temp;
			state.LocalStatistic[nodeCounter].ProcessingTime = temp2;
			nodeCounter++;
		}
	

		//ещё раз для расчета относительной занятости и коэффициента передачи
		for (int i = 0; i < state.LocalStatistic.size(); i++)
		{
			//state.LocalStatistic[i].TransferCoefficient = state.LocalStatistic[i].Probability / probabilitySum;
			state.LocalStatistic[i].TransferCoefficient = (double)WorkTable[i].size() / (double)terminal->GetCount();
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

	FullStatistic StatisticTable::ComputeStatistic(int ammount)
	{
		int currentTime = StateTable.back().back().Time;
		Statistic::FullStatistic state;
		state.ModelInfo.ModelTime = currentTime;
		state.ModelInfo.TransactAmount = ammount;
		int nodeCounter = 0;
		double probabilitySum = 0;

		for each (std::vector<StateStatisticUnit> node in StateTable)
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

			for (int i = 1; i < node.size(); i++)
			{
				//после каждой компоненты делим на время чтобы снизить вероятность переполнения типа
				if (node[i].Time == node[i - 1].Time)
				{
					temp += (node[i].Time - node[lastNotNull].Time) *(node[i].QueueLength - node[i - 1].QueueLength);
					temp3 += (node[i].Time - node[lastNotNull].Time) *(node[i].QueueLength - node[i - 1].QueueLength)
						+ (node[i].Time - node[lastNotNull].Time) * node[i - 1].Busy;
				}
				else
				{
					lastNotNull = i;
					temp += ((node[i].Time - node[i - 1].Time) * node[i - 1].QueueLength);
					temp3 += ((node[i].Time - node[i - 1].Time) * node[i - 1].QueueLength)
						+ (node[i].Time - node[i - 1].Time) * node[i - 1].Busy;
				}
				//считаем количество тактов, которое канал работает
				if (node[i - 1].Busy > 0)
				{
					//время,которое канал находился в одном состоянии * процент загрузки канала
					temp2 += (node[i].Time - node[i - 1].Time)*((double)node[i - 1].Busy / (double)_channel[nodeCounter]);
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

			//	state.LocalStatistic[nodeCounter].TransactAmount =
			//		state.LocalStatistic[nodeCounter].Probability + state.LocalStatistic[nodeCounter].Length;

			//state.LocalStatistic[nodeCounter].WaitTime / state.LocalStatistic[nodeCounter].ProcessingTime;

			nodeCounter++;
		}

		nodeCounter = 0;
		double temp = 0, temp2 = 0;
		for each (std::vector<WorkStatisticUnit> node in WorkTable)
		{
			for (int i = 0; i < node.size(); i++)
			{

				temp += (node[i].Start - node[i].Enter) / double(node.size());
				temp2 += (node[i].Release - node[i].Enter) / double(node.size());
			}
			state.LocalStatistic[nodeCounter].WaitTime = temp;
			state.LocalStatistic[nodeCounter].ProcessingTime = temp2;
			nodeCounter++;
		}


		//ещё раз для расчета относительной занятости и коэффициента передачи
		for (int i = 0; i < state.LocalStatistic.size(); i++)
		{
			//state.LocalStatistic[i].TransferCoefficient = state.LocalStatistic[i].Probability / probabilitySum;
			state.LocalStatistic[i].TransferCoefficient = (double)WorkTable[i].size() / (double)ammount;
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

}