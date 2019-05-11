/*27.01.2017 21:52
������������ ����� ���������.�������� �������� ���������� � ������� ��������� ������,
� ����� ���������� ���������
���� �������� ������� ����, �� ��� ������� ���� ����� n
���� �������� ��� ������, �� ��� ������� ���� ����� -1
09.02.2017 21:39
��������� ���� Cell ������� �������� ��������������� � ������ ����� ������ ���������� ������,
� ������� �������� ���������������. ��� ����� ����� ��������� ��������� ������ ������. ���� �������,
�� � ������� �������� ��������� ����������.
*/
#pragma once
#include "TransactStatistic.cpp"
#include "Network.h"

namespace Model
{
	class Network;
	class Model;

	class Transact
	{
		friend class Model;
	protected:
		int currentNode;
		//-1 ���� �������� ������
		int id;
		static int g_id;
		
		

	public:
		Statistic::TransactStatistic Statistic;
		Transact();
		~Transact();
		//�������� ����, � ������� ��������� ��������
		virtual int GetNode();
		//�������� �� ���������
		virtual int GetId();
		//�������� ���������� ��������� ���������� �� ��� �����
		static int GetAmount();	
		//������� �������� �� ���� matrix � ���������� ����� ����, � ������� ������� ��������.
		virtual int Move(Network* matrix);
		//����� ������, � ������� ������ ��������������� ��������.
		int Cell;
		//���������, ����� �������� �������� � ����
		virtual void OnStart(const int& time);
		//����������, ����� �������� ������� ����
		virtual void OnStop(const int& time);


	};
}

