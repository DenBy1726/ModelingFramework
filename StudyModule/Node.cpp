#pragma once
#include "Node.h"
#include "FIFOQueue.cpp";
namespace Model
{

	Node::Node(Random::IDistribution<double>* dist, int capacity, Queue::IQueue<Transact*>* q)
	{
		this->generator = dist;
		this->Queue = q;
		this->channel = capacity;
		this->capacity = capacity;
	}


	Node::~Node()
	{
		Clear();
		delete this->Queue;
		delete this->generator;
	}

	std::vector<Statistic::StateStatisticUnit> Node::GetStateStatistic()
	{
		return this->Statistic;
	}

	std::vector<Statistic::WorkStatisticUnit> Node::GetWorkStatistic()
	{
		return this->WorkStatistic;
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
		OnStart(haveEntered->Cell, time);
		int timeToProcessed = time + this->generator->Next();
		//������� ��������� � �����: ������� ����� + ����� ���������
		return new Event(EventId::Processed, haveEntered, timeToProcessed);
	}

	///���������� ��� ��������� �1
	//���������� ������ ����� ���� �� ��������,����� ����� � �������. � ������ ���� ��������
	//����� ����� ���������� ������� ������������ ������.
	Event* Node::TryCapture(Transact* haveEntered,const int& time)
	{
		
		Event* happened = nullptr;
		if (this->capacity == 0)
		{
			this->Queue->Add(0, haveEntered);
		}
		else
		{
			happened = Capture(haveEntered, time);
		}
		UpdateState(time);
		return happened;
	}

	//������������ �������� � ����� �������� � �������� ������� ������ ���
	Event* Node::BeginCapture(Transact* haveEntered, const int& time)
	{
		OnEnter(haveEntered->GetId(), time);
		haveEntered->Cell = WorkStatistic.size() - 1;
		return TryCapture(haveEntered, time);
	}

	void Node::Register(Transact* haveEntered, const int& time)
	{
		OnEnter(haveEntered->GetId(), time);
		haveEntered->Cell = WorkStatistic.size() - 1;
	}


	///���������� ��� ��������� �2
	//����������� ����� � ���� � ����� ���� ��������� ����� ������, ���������� �������
	//������������ ������
	Event* Node::Release(Transact* released,const int& time)
	{
		Event* willHappen = nullptr;
		OnRelease(released->Cell,time);
		released->Cell = -1;
		//���� � ������ ���� �������, �� ����� ��������� ������, �������� �����, � ����������
		//������� ������������ ������
		if (Queue->IsEmpty() == false)
		{
			Transact* newTransact = Queue->Remove();
			willHappen = Capture(newTransact, time);
		}
		UpdateState(time);
		return willHappen;
	}


	//��������� ����� �������� � ������� numberOfTransact ����� � �����
	void Node::OnEnter(const int& id,const int& time)
	{
		WorkStatistic.push_back(Statistic::WorkStatisticUnit());
		WorkStatistic[WorkStatistic.size()-1].Enter = time;
	}


	//��������� ����� �������� � ������� numberOfTransact �������� ���������
	void Node::OnStart(const int& statId, const int& time)
	{
		WorkStatistic[statId].Start = time;
		this->capacity--;
	}


	//��������� ����� �������� � ������� numberOfTransact ����������� �����
	void Node::OnRelease(const int& statId, const int& time)
	{
		WorkStatistic[statId].Release = time;
		this->capacity++;
	}

	//��������� ����� �������� �������� ���������
	void Node::OnStart(Transact* tr, const int& time)
	{
		WorkStatistic[tr->Cell].Start = time;
		this->capacity--;
	}
	//��������� ����� �������� ��������� ���������
	void Node::OnRelease(Transact* tr, const int& time)
	{
		WorkStatistic[tr->Cell].Release = time;
		this->capacity++;
	}

	//������� ������
	void Node::Clear()
	{
		this->Statistic.clear();
		this->WorkStatistic.clear();
		this->capacity = channel;
	}

	//���������� ������ ��� ����� ���������� ��������� ������ ��� �������
	void Node::UpdateState(const int& time)
	{
		Statistic::StateStatisticUnit temp;	
		temp.Busy = channel - this->capacity;
		temp.Time = time;
		temp.QueueLength = this->Queue->Length();
		this->Statistic.push_back(temp);
	}

	Descriptors::NodeDescriptor Node::GetDescriptor()
	{
		return Descriptors::NodeDescriptor(Descriptors::DistributionDescriptor(generator->Type(), generator->Argument()),capacity,this->Queue->Type());
	}


}