/*
22.01.2017 0:46
Генератор по равномерному распределению для вещественных чисел
*/
#pragma once
#include "IDistribution.h"
#include "Random.cpp"
namespace Random
{

	class UniformRealDistribution : public IDistribution<double>
	{
		std::uniform_real_distribution<> _dist;
	public:

		UniformRealDistribution(const Random& gen = Random())
		{
			_gen = gen;
		}

		UniformRealDistribution(const std::vector<double>& arg, const Random& gen = Random())
		{
			_gen = gen;
			_dist = std::uniform_real_distribution<>(arg[0], arg[1]);
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
			return "UniformRealDistribution";
		}

		std::vector<double> Argument()
		{
			return{ _dist.param().a(), _dist.param().b() };
		}

		IDistribution<double>* Clone(const std::vector<double>& arg)
		{
			return new UniformRealDistribution(arg);
		}
	};
}