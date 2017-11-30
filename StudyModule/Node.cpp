#pragma once
#include "Node.h"
namespace Model
{

	Node::Node(Random::IDistribution<double>* dist, int capacity)
	{
		this->Distribution = dist;
		this->channel = capacity;
		this->capacity = capacity;
	}

	Node::Node(int capacity)
	{
		this->channel = capacity;
		this->capacity = capacity;
	}


	Node::~Node()
	{
		Clear();
		//delete this->Queue;
		//delete this->Distribution;
	}



	//����� �� ����� ���������
	bool Node::IsBusy()
	{
		return capacity == 0;
	}

	//��������� �������
	int Node::Capacity()
	{
		return capacity;
	}

	//���������� �������
	int Node::Channel()
	{
		return channel;
	}
	//������ ������ � ���������� ������� ���� �2
	Event* Node::Capture(Transact* haveEntered, const int& time)
	{
		
		//OnStart(haveEntered->Cell, time);
		this->capacity--;
		int timeToProcessed = time + this->Distribution->Next();
		//������� ��������� � �����: ������� ����� + ����� ���������
		return new Event(EventId::Processed, haveEntered, timeToProcessed);
	}

	///���������� ��� ��������� �1
	//���������� ������ ����� ���� �� ��������,����� ����� � �������. � ������ ���� ��������
	//����� ����� ���������� ������� ������������ ������.
/*	Event* Node::TryCapture(Transact* haveEntered, const int& time)
	{
		
		Event* happened = nullptr;
		if (this->capacity == 0)
		{
			this->Queue->Add(haveEntered);
		}
		else
		{
			happened = Capture(haveEntered, time);
		}
		UpdateState(time);
		return happened;
	}*/
	


	///���������� ��� ��������� �2
	//����������� ����� � ���� � ����� ���� ��������� ����� ������, ���������� �������
	//������������ ������
	void Node::Release()
	{
		if (this->capacity < this->channel)
			this->capacity++;
		//OnRelease(released->Cell,time);
		//���� � ������ ���� �������, �� ����� ��������� ������, �������� �����, � ����������
		//������� ������������ ������
	
		//UpdateState(time);

	}


	//��������� ����� �������� � ������� numberOfTransact ����� � �����
	

	int Node::GetId()
	{
		return id;
	}




	//������� ������
	void Node::Clear()
	{
		this->capacity = channel;
	}




}