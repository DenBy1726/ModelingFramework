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
#include "Time.h"
#include "IDistribution.h"
#include "Transact.h"
#include "IQueue.h"
#include "Event.h"
#include "DistributionBuilder.h"
#include "NodeQueue.h"
#include "NodeList.h"
#include <vector>
namespace Model
{
	class Transact;
	struct Event;
	class NodeQueue;
	class NodeList;
	class Node
	{
		friend class Model;
		friend class NodeList;
		

	private:

		int channel;
		int capacity;	
		int id;
	
	public:
		
		
		NodeQueue* Queue;
		Random::IDistribution<double>* Distribution;

		Node(int capacity);

		Node(Random::IDistribution<double>* dist, int capacity = 1);

		~Node();

		//���������� ������ ����� ���� �� ��������,����� ����� � �������. 
	//	Event* TryCapture(Transact* haveEntered, const int& time);
		//������ ������
		Event* Capture(Transact* haveEntered, const int& time);

		//����������� ����� � ���� � ����� ���� ��������� ����� ������, ���������� �������
		//������������ ������.��� ����� �� ����� �������� �������, time �������� ������� �����.
		void Release();
		//������� ������
		void Clear();

		//�������� �� �����
		bool IsBusy();

		//��������� �������
		int Capacity();

		int Channel();

		int GetId();



	};
}