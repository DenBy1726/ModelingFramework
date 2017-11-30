/*
22.01.2017 0:36
Генератор по равномерному распределению
*/
#pragma once
#include "IDistribution.h"
#include "Random.cpp"
namespace Random
{

	class UniformDistribution : public IDistribution<double>
	{
		std::uniform_int_distribution<> _dist;
	public:

		UniformDistribution(const Random& gen = Random())
		{
			_gen = gen;
		}

		//аргумент 1 - минимум
		//аргумент 2 - максимум
		UniformDistribution(const std::vector<double>& arg, const Random& gen = Random())
		{
			_gen = gen;
			_dist = std::uniform_int_distribution<>(arg[0], arg[1]);
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
			return "UniformDistribution";
		}

		std::vector<double> Argument()
		{
			return{ (double)_dist.param().a(),(double)_dist.param().b() };
		}

		IDistribution<double>* Clone(const std::vector<double>& arg)
		{
			return new UniformDistribution(arg);
		}
	};
}