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
	class Terminal;
	class Transact
	{
		friend class Generator;
		friend class Terminal;
	protected:
		int currentNode;
		//-1 ���� �������� ������
		int id;
		static int g_id;
		
		//���������, ����� �������� �������� � ����
		virtual void OnStart(const int& time);
		//����������, ����� �������� ������� ����
		virtual void OnStop(const int& time);

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



	};
}

