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
#include "Queue.cpp"
#include "Descriptors.cpp"
#include "DistributionBuilder.h"

#include <vector>

namespace Model
{
	class Node;
	class Network
	{
	private:
		Random::IRandom* generator;
	public:
		//������� ��������
		BasicMatrix<double> TransitionMatrix;
		//������ ������� ����
		std::vector<Node*> Channel;

		~Network();
		
		//handler - ���������� ����
		Network(const BasicMatrix<double>& transition, const std::vector<Node*> nodes);
		//�������� ����� ������, � ������� �������� �������� �� current
		int GetNextChannel(const int& current);
		//�������� ����
		void Clear();
		//��������� �������� �� current ���� ������������
		bool IsTerminator(const int& current);

		Descriptors::NetworkDescriptor GetDescriptor();
	};

	
}