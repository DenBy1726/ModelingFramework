/*
18.02.2017 14:37
����������� ������� ������������ ��� ��������������� ������ �� ������������� �������� � ����������
�� � �����. ���������� ���������� ������������, ��� ������, ����������� ��� ������ ������ Save �����
���� � �������� ������������� ������� Load 
(BETA)
- ����������� ������ ��������
04.04.2017 19:48
������ ������ �����������
������������ ������� ����������� �������������
*/
#pragma once
#include "IDistribution.h"
#include "Model.h"
#include "Node.h"
#include <string>
#include <map>
namespace Model
{
	//������� ������ ������������, ������ ����� ���� ��������� �������������
	//����� ��������� ��������� � ��������� ������ ������ � ���������������� � ���������� ��������������
	//����� �� �������� �� �������,������������ � ������, ����� �������������� 2-3Heap
	class ModelBuilder
	{
		
	public:
		//������� ��� ����������� ���������� �������� ������������� � ���
		std::map<std::string, std::string> Command;
		ModelBuilder();
		//������ ������ �� ������ ��������� ��������
		virtual Model* Load(const std::string&,Queue::QueueType QT = Queue::Heap23);
		//��������� ������ � ������ ��� ����������� � ������� ����������� ��� ������ ����� ModelBuilder
		virtual std::string Save(const Model& data);

	protected:
		//������� ���������� �� ���������� ��������
		virtual Descriptors::DistributionDescriptor CreateGenerator(const std::string&);
		//������� ��������� �������� �� �����������
		virtual std::string SaveGenerator(const Descriptors::DistributionDescriptor&);

	};
}