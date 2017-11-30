/*27:01:2017 22:19
�������� ������������ ����� ����. ���� ��� ����������, ������� ����������� ������ � ������� 
���������.������ ��� �������������� ������ ����(������� � �������) � ����� ��� ������� ����������
������, ���� �������� ��������.����� ������� ����� � ������� 0 ���������� ��� ��������� ������
������ � �������� 1..n-1 ��� �������������� ����, � ����� � ������� n ��� ����������.
////////////////////
18.02.2017 15:11
������ ���������� ������������ ������ �������� ������� �������� ����������, ��� ����������
///////////////////
11.03.2017 11:35
���� ����� ������� ���� ���������,��� �������� ��������� � ��������� ����
*/
#pragma once
#include "matrix.h"
#include "Node.h"
#include "IRandom.h"

#include "DistributionBuilder.h"
#include "Generator.h"
#include "Terminal.h"
#include "Time.h"
#include "Transact.h"
//#include "Statistic.cpp"
#include "IQueue.h"
#include "EventQueue.h"
#include "NodeList.h"
#include <vector>

namespace Model
{
	class Node;
	class Generator;
	class Terminal;
	class EventQueue;
	class NodeList;
	class Network
	{
	private:
		Random::IRandom* _generator;
		
	public:
		
		//������� ��������
		BasicMatrix<double> TransitionMatrix;
		//������ ������� ����
		
		
		Generator* Generator;
		Terminal* Terminal;
		EventQueue* EventQueue;
		NodeList* Channel;

		Time* Time;

		~Network();
		
		//handler - ���������� ����
		Network(const BasicMatrix<double>& transition, NodeList* nodes);
		Network();
		//�������� ����� ������, � ������� �������� �������� �� current
		int GetNextChannel(const int& current);
		//�������� ����
		void Clear();
		//��������� �������� �� current ���� ������������
		bool IsTerminator(const int& current);

		void MoveTime();



	};

	
}