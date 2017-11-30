/*
22.01.2017 0:56
��������� �� ������������� �������������.
������������� ���������� �������� � ������������������ �� n
����������� ��������� �������������, �����, ��� ����������� ������� � ������ �� ��� 
��������� � ����� p
*/
#pragma once
#include "IDistribution.h"
#include "Random.cpp"
namespace Random
{

	class BinomialDistribution : public IDistribution<double>
	{
		std::binomial_distribution<> _dist;
	public:

		BinomialDistribution(const Random& gen = Random())
		{
			_gen = gen;
		}

		//�������� 1 - ���������� ���������
		//�������� 2 - ���������� ������
		BinomialDistribution(const std::vector<double>& arg, const Random& gen = Random())
		{
			_gen = gen;
			_dist = std::binomial_distribution<>(arg[0], arg[1]);
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
			return "BinomialDistribution";
		}

		std::vector<double> Argument()
		{
			return{ (double)_dist.param().t(), _dist.param().p() };
		}

		IDistribution<double>* Clone(const std::vector<double>& arg)
		{
			return new BinomialDistribution(arg);
		}
	};
}