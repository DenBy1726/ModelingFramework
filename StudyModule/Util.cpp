/*
05.04.2017 14:50
��������� ������ ��� ������ � ������� � �������� ������
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
		//����� ��� �������� �������
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

		//������������ ������ ���� �������
		enum EventId{ Free, Processed };

		//��������� ��� �������� �������.
		//������� ������ ���, ����� ������������� � ��������� ��������
		struct Event
		{


			EventId Id;
			//Event ������ ������ ���������,� �� ������� ���, � �������������
			//�� ������ �������� �� �����������.
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
			//��������� ��������
			Random::IDistribution<double>* Generator;
			//������� � ������
			Queue::FIFOQueue<Transact*> Queue;
			//���� ���������
			bool IsWorking;
			//���������� ���������
			std::vector<Statistic::StateStatisticUnit> StateStatistic;
			//���������� ������
			std::vector<Statistic::WorkStatisticUnit> WorkStatistic;

			Node()
			{
				IsWorking = false;
			}

		};

		
		class Network
		{
		public:
			//��������� ��� ������� ��������� ����������
			Random::Random Generator;
			//��������� ��� ����������� ��������� ������ � ����
			Random::IDistribution<double>* EnterGenerator;
			//������� ��������
			BasicMatrix<double> Matrix;
			//������ ������� ����
			std::vector<Node> Nodes;
			//������� ��������� �����
			Struct::Time Time;
			//������� �������
			Queue::PriorityQueue<Struct::Event*> Queue;

			Network()
			{

			}


		};

		class Transact
		{
		public:
			//����� ���� � ������� ������ ��������� ��������
			int CurrentNode;
			//����� ������, � WorkStatistic � ������� ��������������� ��������.
			int Cell;
			//������������� ���������.�������� �������������
			int Id;
			//���������� ���������
			Statistic::TransactStatistic Statistic;


			Transact()
			{
				this->Id = -1;
				this->CurrentNode = -1;
				this->Cell = -1;
				//currentNode = 0;//��������
			}



		};


	}

	


}
