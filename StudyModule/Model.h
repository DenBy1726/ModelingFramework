/*28.01.2017 19:59
������������ ����� ������. ������ ��������� ������������ ��������� ��������� �������������. � ������
���������������, ���������, �������� ����������.
////////////////////
18.02.2017 15:11
������ ���������� ������������ ������ �������� ������� �������� ����������, ��� ����������
///////////////////
11.03.2017 11:28
������ ����� ������� ���� ���������,��� �������� ��������� � ��������� ������
05.05.2017 13:41
������� DistributionBuilder ��� ��������� ���������������� �������������
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

		//��������� ������� �������������( ��������������� ��������� �������)
		void StartModeling();
		//��������� ������� happened
		void HandleEvent(Event* happened);
		//��������� ������� ������� ���� ��� ��������� entering
		void EventEnter(Transact* entering);
		//��������� ������� ������� ���� ��� ��������� releasing
		void EventRelease(Transact* releasing);
		//��������� ��������
		void AddTransact();

	public:
		Model(const BasicMatrix<double>& transitionMatrix,
			const std::vector<Node*>& nodeGenerator,Random::IDistribution<double>* modelGenerator,
			const Queue::QueueType& QT);


		~Model();
		//��������� ������� ������������� �� ammountOf ����������
		void Start(int ammountOf);
		//������� ������
		void Clear();
		//���������� ������� ��������� �����
		int GetCurrentTime();
		//���������� ����������� ���������� � ������� ����
		Statistic::FullStatistic ComputeStatistic();

		Descriptors::ModelDescriptor GetDescriptor() const;
		

	};


}