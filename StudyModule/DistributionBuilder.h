/*
04.04.2017
������� �������������.
����� ���� � distribution.cpp
������������ ������������� �� ���� � ����������
12.04.2017 10:02
������ ����������� �����
*/
#pragma once
#include "IDistribution.h"
#include "Descriptors.cpp"
#include <map>
#include <string>

namespace Random
{
	
	static class DistributionBuilder
	{
	public:
		static std::map<std::string, IDistribution<double>* > Distribution;
		
		
		static IDistribution<double>* Create(const std::string& DT, const std::vector<double>& arg);
		//�������� ���������� ������������� �� �����������
		static IDistribution<double>* Create(const Descriptors::DistributionDescriptor& DT);

	};
	typedef DistributionBuilder Distribution;
}