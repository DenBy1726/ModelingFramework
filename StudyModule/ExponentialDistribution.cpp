/*
26.01.2016 16:15
��������� �� ����������������� �������������.
��������� ����������� �������������, ������������ ����� ����� ����� ����������������� 
����������� ������ � ���� �� �������.
*/
#pragma once
#include "IDistribution.h"
#include "Random.cpp"
namespace Random
{

	class ExponentialDistribution : public IDistribution<double>
	{
		std::exponential_distribution<> _dist;
	public:

		ExponentialDistribution(const Random &gen = Random())
		{
			_gen = gen;
		}


		//�������� 1- ������
		ExponentialDistribution(const std::vector<double>& arg, const Random& gen = Random())
		{
			_gen = gen;
			_gen = gen;
			double lambda = arg[0];
			if (lambda > 1)
				lambda = 1.0 / arg[0];
			_dist = std::exponential_distribution<>(lambda);
		}
		double Next()
		{
			return _dist(_gen.Engine());
		}
		Random Generator()
		{
			return _gen;
		}

		std::string Type()
		{
			return "ExponentialDistribution";
		}

		std::vector<double> Argument()
		{
			return{ _dist.param().lambda() };
		}

		IDistribution<double>* Clone(const std::vector<double> &arg)
		{
			return new ExponentialDistribution(arg);
		}
	};
}