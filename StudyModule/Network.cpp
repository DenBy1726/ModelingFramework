#include "Network.h"

namespace Model
{
	Network::Network(const BasicMatrix<double>& transition,NodeList* nodes)
	{
		this->TransitionMatrix = transition;
		if (this->TransitionMatrix.width() != this->TransitionMatrix.height())
			throw ("������� ��������� ������ ���� ����������");
		//�������� 2 ��� ��� ������� ��� ���������, � ��������� ��� ����������. ��� �������� ������.
		if (nodes->Length() != this->TransitionMatrix.width() - 2)
			throw ("���������� ����������� ������ �������� ���������� �����");
		this->Channel = nodes;
		this->_generator = new ::Random::Random();
	}

	Network::Network()
	{
		this->_generator = new ::Random::Random();
	}


	Network::~Network()
	{
		Clear();
		delete this->_generator;
		
	}


	//�������� ����� ������, � ������� �������� �������� �� current
	/*
		���� ��������� � ��� ��� ������� ������������ ����� 0 <= p <= 1
		����� ��������� �������
		���� �� ������������ �� ������� ����, ��������� � ������� � ����
		����� ������ p �� ���������� i -�� ����.
		������:
		������� : 0 0.5 0.5 0 0
		0 0   0   0 1
		0 0   0   1 0
		0 0.7 0   0 0.3
		0 0   0   0 0
		������� ����: 0
		p: 0.78

		���������� ������� � ��������, ������� ������ � ������� ����
		1:   0 + 0.5 = 0.5 < 0.78
		2:   0.5 + 0.5 = 1 > 0.78 ��������� � 2
		*/
	int Network::GetNextChannel(const int& current)
	{
		//����� ��� ����������� ������ ����
		double probValue = this->_generator->NextDouble();
		int length = this->TransitionMatrix.height();
		double probSum = 0;
		//� ������ ��� ��� ������ ������� �� ��������
		for (int i = 1; i < length; i++)
		{
			probSum += this->TransitionMatrix[current][i];
			if (probSum > probValue)
				return i;
		}
		throw("����� ������������ ��������� � ������ ������� �� ����� 1");
	}

	//�������� ����
	void Network::Clear()
	{
		for (int i = 0; i < Channel->Length(); i++)
		{
			Channel->operator[](i)->Clear();
		}
		this->TransitionMatrix = BasicMatrix<double>();
		this->Terminal->Clear();
		this->Generator->Clear();
	}

	//��������� �������� �� current ���� ������������
	//���������� ���� ����� ���� n ,��� n ����������� �������
	bool Network::IsTerminator(const int& current)
	{
		if (current == this->TransitionMatrix.height()-1)
			return true;
		else
			return false;
	}

	void Network::MoveTime()
	{
		if (EventQueue->Empty() == false)
			Time->time = EventQueue->Top()->Time;
	}

}