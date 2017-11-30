/*
22.01.2017 0:36
Генератор бернулли.
дискретное распределение вероятностей, моделирующее случайный эксперимент 
произвольной природы, при заранее известной вероятности успеха или неудачи
*/
#pragma once
#include "IDistribution.h"
#include "Random.cpp"
namespace Random
{

	class BernoulliDistribution : public IDistribution<double>
	{
		std::bernoulli_distribution _dist;
	public:

		BernoulliDistribution(const Random& gen = Random())
		{
			_gen = gen;
		}

		//аргумент 1 - вероятность успеха
		BernoulliDistribution(const std::vector<double> &arg ,const Random& gen = Random())
		{
			_gen = gen;
			_dist = std::bernoulli_distribution(arg[0]);
		}
		double Next()
		{
			return (bool)_dist(_gen.Engine());
		}
		Random Generator()
		{
			return _gen;
		}

		std::string Type()
		{
			return "BernoulliDistribution";
		}

		std::vector<double> Argument()
		{
			return{ _dist.param().p() };
		}

		IDistribution<double>* Clone(const std::vector<double>& arg)
		{
			return new BernoulliDistribution(arg);
		}


	};
}