/*27:01:2017 22:25
�������� ������������ ����� ����.���� �������� ���������, ��� ������ �������� ������������
����� �� ��������� ��������� ��������� ���������, ������� ���������� � ����, ���� ���������
������, � ����� �������������� ������.
////////////////////
09.02.2017 21:10
������� ���������:
����� BeginCapture(Transact,const int&) ��� ������ ������� ������
����� TryCapture ������ �������� ������ �� ��������������� ���������� �������� � ����������
(����� � ������� ��� ������ �����)
����� Capture �������� �� ������� ������.
������� ���� isWorked. ������ ������ �������� ��� ������ ����� ������, � ������� ������ � � �����
� ���� ����� Release ������ � �������� ��������� ������ ��������� ��������� �� ������������� ��������.
������� ���������� ������ �������� �� � ��� �������(��������� �� ���������), � � �������.
////////////////////
18.02.2017 15:11
������ ���������� ������������ ������ �������� ������� �������� ����������, ��� ����������
07.04.2017 20:35
�������� ����� ����������� ��������� � ������, ��� ��������� ������� ������� ������� �� �����
�������� ����� �������� ��������� ������
�������� ��� ���������� ��� �������� ������
/////////////////////
11.04.2017 21:05
������� �� �������������� ����������. 
����� IsWorking() ������� �� IsBusy()
bool isWorking ������� �� int capacity.�������� ���������� ��������� �������. ���� 0 �� ���������� ������
����� Capasity() ���������� ������� ��������� ������� ������
������ ����� �������� ��� ������ NodeDescriptor. ����� DistributionDescriptor
*/
#pragma once
#include "IDistribution.h"
#include "Transact.h"
#include "IQueue.h"
#include "FifoQueue.cpp"
#include "Event.h"
#include "Descriptors.cpp"
#include "StateStatisticUnit.cpp"
#include "WorkStatisticUnit.cpp"
#include "DistributionBuilder.h"
#include <vector>
#include <map>
namespace Model
{
	class Transact;
	struct Event;

	class Node
	{
		friend class Model;
	private:
		Random::IDistribution<double>* generator;
		
		int channel;
		int capacity;
		std::vector<Statistic::StateStatisticUnit> Statistic;
		std::vector<Statistic::WorkStatisticUnit> WorkStatistic;
		
		//��������� ����� �������� � ������� numberOfTransact ������ � �����
		void OnEnter(const int& id,const int& time);
	

	public:
		Queue::IQueue<Transact*>* Queue;

		Node(Random::IDistribution<double>* dist, int capacity = 1, Queue::IQueue<Transact*>* q = new Queue::FIFOQueue<Transact*>());

		~Node();
		//�������� ���� ������� ������ � ������ ������� �������
		Event* BeginCapture(Transact* haveEntered, const int& time);
		//���������� ������ ����� ���� �� ��������,����� ����� � �������. 
		Event* TryCapture(Transact* haveEntered,const int& time);
		//������ ������
		Event* Capture(Transact* haveEntered, const int& time);

		//����������� ������ � ������
		void Register(Transact* haveEntered, const int& time);

		//��������� ����� �������� � ������� numberOfTransact �������� ���������
		void OnStart(const int& statId, const int& time);
		//��������� ����� �������� � ������� numberOfTransact ��������� ���������
		void OnRelease(const int& statId, const int& time);

		//��������� ����� �������� �������� ���������
		void OnStart(Transact* tr, const int& time);
		//��������� ����� �������� ��������� ���������
		void OnRelease(Transact* tr, const int& time);

		std::vector<Statistic::StateStatisticUnit> GetStateStatistic();
		std::vector<Statistic::WorkStatisticUnit> GetWorkStatistic();

		//������ � ���������� ���������� � ����� ���������
		void UpdateState(const int& time);
		
		//����������� ����� � ���� � ����� ���� ��������� ����� ������, ���������� �������
		//������������ ������.��� ����� �� ����� �������� �������, time �������� ������� �����.
		Event* Release(Transact* released,const int& time);
		//������� ������
		void Clear();

		//�������� �� �����
		bool IsBusy();

		//��������� �������
		int Capacity();

		int Channel();

		//���������� ����������, ������� �������� ���������� �� �������,����� ����� ����� ���� ������������
		Descriptors::NodeDescriptor GetDescriptor();

	};
}