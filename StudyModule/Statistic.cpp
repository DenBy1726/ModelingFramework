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

	//��������� ����� �������� �������� ���������
	void StatisticTable::OnStart(Model::Node* node, Model::Transact* tr, const int& time)
	{
		WorkTable[node->GetId()][tr->Cell].Start = time;
	}
	//��������� ����� �������� ��������� ���������
	void StatisticTable::OnRelease(Model::Node* node, Model::Transact* tr, const int& time)
	{
		WorkTable[node->GetId()][tr->Cell].Release = time;
		tr->Cell = -1;
	}

	//���������� ������ ��� ����� ���������� ��������� ������ ��� �������
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
			//������� ������ �������
			// = �����(mi * li) / n
			// li ������ ������� � i ���������
			// mi ������������ i ���������
			// n ���������� �������
			//������������� ������ ��
			//���������� ������, ����� ���������� ������ / ������� ����� ������
			long double temp = 0;
			double temp2 = 0;
			double temp3 = 0;
			int lastNotNull = 0;

			for (int i = 1; i < node.size(); i++)
			{
				//����� ������ ���������� ����� �� ����� ����� ������� ����������� ������������ ����
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
				//������� ���������� ������, ������� ����� ��������
				if (node[i - 1].Busy > 0)
				{
					//�����,������� ����� ��������� � ����� ��������� * ������� �������� ������
					temp2 += (node[i].Time - node[i - 1].Time)*((double)node[i - 1].Busy / (double)_channel[nodeCounter]);
				}
			}

			state.LocalStatistic[nodeCounter].Length = temp / double(currentTime);
			state.LocalStatistic[nodeCounter].Probability = temp2 / double(currentTime);
			state.LocalStatistic[nodeCounter].TransactAmount = temp3 / double(currentTime);

			probabilitySum += state.LocalStatistic[nodeCounter].Probability;
			//������� ����� �������� w
			// = �����(���� ������� - ���� �����)
			//������� ����� ��������� � ������� u
			// = �����(���� ������������ - ���� �����
			temp = 0;
			temp2 = 0;
			//it - ���� <����� ���������,������� ���������� ������� ���������>
			// ������� ���������� - ���� �����, �������, ������������
		
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
	

		//��� ��� ��� ������� ������������� ��������� � ������������ ��������
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
			//������� ������ �������
			// = �����(mi * li) / n
			// li ������ ������� � i ���������
			// mi ������������ i ���������
			// n ���������� �������
			//������������� ������ ��
			//���������� ������, ����� ���������� ������ / ������� ����� ������
			long double temp = 0;
			double temp2 = 0;
			double temp3 = 0;
			int lastNotNull = 0;

			for (int i = 1; i < node.size(); i++)
			{
				//����� ������ ���������� ����� �� ����� ����� ������� ����������� ������������ ����
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
				//������� ���������� ������, ������� ����� ��������
				if (node[i - 1].Busy > 0)
				{
					//�����,������� ����� ��������� � ����� ��������� * ������� �������� ������
					temp2 += (node[i].Time - node[i - 1].Time)*((double)node[i - 1].Busy / (double)_channel[nodeCounter]);
				}
			}

			state.LocalStatistic[nodeCounter].Length = temp / double(currentTime);
			state.LocalStatistic[nodeCounter].Probability = temp2 / double(currentTime);
			state.LocalStatistic[nodeCounter].TransactAmount = temp3 / double(currentTime);

			probabilitySum += state.LocalStatistic[nodeCounter].Probability;
			//������� ����� �������� w
			// = �����(���� ������� - ���� �����)
			//������� ����� ��������� � ������� u
			// = �����(���� ������������ - ���� �����
			temp = 0;
			temp2 = 0;
			//it - ���� <����� ���������,������� ���������� ������� ���������>
			// ������� ���������� - ���� �����, �������, ������������

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


		//��� ��� ��� ������� ������������� ��������� � ������������ ��������
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