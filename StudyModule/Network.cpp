#include "Network.h"

namespace Model
{
	Network::Network(const BasicMatrix<double>& transition, const std::vector<Node*> nodes)
	{
		this->TransitionMatrix = transition;
		if (this->TransitionMatrix.width() != this->TransitionMatrix.height())
			throw ("������� ��������� ������ ���� ����������");
		//�������� 2 ��� ��� ������� ��� ���������, � ��������� ��� ����������. ��� �������� ������.
		if (nodes.size() != this->TransitionMatrix.width() - 2)
			throw ("���������� ����������� ������ �������� ���������� �����");
		this->Channel = nodes;
		this->generator = new Random::Random();
	}


	Network::~Network()
	{
		Clear();
		delete this->generator;
		for each(Node* node in this->Channel)
		{
			delete node;
		}
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
		double probValue = this->generator->NextDouble();
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
		for each(Node* node in this->Channel)
		{
			node->Clear();
		}
		this->Channel.clear();
		this->TransitionMatrix = BasicMatrix<double>();
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

	Descriptors::NetworkDescriptor Network::GetDescriptor()
	{
		std::vector<Descriptors::NodeDescriptor> nodeDescriptors;
		for each(auto it in this->Channel)
		{
			nodeDescriptors.push_back(it->GetDescriptor());
		}
		return Descriptors::NetworkDescriptor(this->TransitionMatrix, nodeDescriptors);
	}

}