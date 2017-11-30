/*
22.01.2017 0:56
Генератор по биномиальному распределению.
распределение количества «успехов» в последовательности из n
независимых случайных экспериментов, таких, что вероятность «успеха» в каждом из них 
постоянна и равна p
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

		//аргумент 1 - количество испытаний
		//аргумент 2 - веротность успеха
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