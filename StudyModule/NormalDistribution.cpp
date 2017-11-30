/*
26.01.2017 16:15
������������� ������������, ������� ������ ��������� ���� �� ������ �������� ������, 
�������� � ������. ���������� �������� ����������� ����������� �������������, 
����� ��� ���������� ������� ��������� ����� ��������� �����
*/
#pragma once
#include "IDistribution.h"
#include "Random.cpp"
namespace Random
{

	class NormalDistribution : public IDistribution<double>
	{
		std::normal_distribution<> _dist;
	public:

		NormalDistribution(const Random& gen = Random())
		{
			_gen = gen;
		}

		//�������� 1 - ��
		//�������� 2 - �����
		NormalDistribution(const std::vector<double>& arg, Random gen = Random())
		{
			_gen = gen;
			_dist = std::normal_distribution<>(arg[0], arg[1]);
		}
		double Next()
		{
			return (int)_dist(_gen.Engine());
		}
		Random Generator()
		{
			return _gen;
		}

		std::string Type()
		{
			return "NormalDistribution";
		}

		std::vector<double> Argument()
		{
			return{ _dist.param().mean(), _dist.param().sigma() };
		}

		IDistribution<double>* Clone(const std::vector<double>& arg)
		{
			return new NormalDistribution(arg);
		}
	};
}