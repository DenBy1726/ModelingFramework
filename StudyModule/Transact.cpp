#pragma once
#include "Transact.h"

namespace Model
{
	int Transact::g_id = 0;

	Transact::Transact()
	{
		this->id = Transact::g_id++;
		//currentNode = 0;//��������
	}

	Transact::~Transact()
	{
		this->id = -1;
		this->currentNode = -1;
	}

	//�������� ����, � ������� ��������� ��������
	int Transact::GetNode()
	{
		return this->currentNode;
	}

	//�������� �� ���������
	int Transact::GetId()
	{
		return this->id;
	}

	//�������� ���������� ��������� ���������� �� ��� �����
	int Transact::GetAmount()
	{
		return g_id;
	}

	//���������, ����� �������� �������� � ����
	//���������� �������
	void Transact::OnStart(const int& time)
	{
		this->Statistic.Start = time;
		currentNode = 0;
	}

	//����������, ����� �������� ������� ����
	//���������� �������
	void Transact::OnStop(const int& time)
	{
		this->Statistic.Finish = time;
	}

	//������� �������� �� ���� matrix � ���������� ����� ����, � ������� ������� ��������.
	int Transact::Move(Network* matrix)
	{
		currentNode = matrix->GetNextChannel(currentNode);
		return currentNode;
	}

}