/*
26.01.2016 16:15
Генератор по эксаоненциальному распределению.
абсолютно непрерывное распределение, моделирующее время между двумя последовательными 
свершениями одного и того же события.
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


		//аргумент 1- лямбда
		ExponentialDistribution(const std::vector<double>& arg, const Random& gen = Random())
		{
			_gen = gen;
			_dist = std::exponential_distribution<>(arg[0]);
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