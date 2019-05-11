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

	class IntExponentialDistribution : public IDistribution<double>
	{
		std::exponential_distribution<> _dist;
	public:

		IntExponentialDistribution(const Random &gen = Random())
		{
			_gen = gen;
		}

		////аргумент 1- лямбда
		IntExponentialDistribution(const std::vector<double>& arg, const Random& gen = Random())
		{
			_gen = gen;
			double lambda = arg[0];
			if (lambda > 1)
				lambda = 1.0 / arg[0];
			_dist = std::exponential_distribution<>(lambda);
			auto x = _dist.lambda();
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
			return "IntExponentialDistribution";
		}

		std::vector<double> Argument()
		{
			return{ _dist.param().lambda() };
		}

		IDistribution<double>* Clone(const std::vector<double>& arg)
		{
			return new IntExponentialDistribution(arg);
		}
	};
}