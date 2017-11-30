#pragma once
#include "DistributionBuilder.h"
#include "Distribution.cpp"
namespace Random
{
	


	IDistribution<double>* DistributionBuilder::Create(const std::string& DT, const std::vector<double>& arg)
	{
		return Distribution[DT]->Clone(arg);
	}


	std::map<std::string, IDistribution<double>* > DistributionBuilder::Distribution
	{
		{ "EXP", new IntExponentialDistribution() },
		{ "NORMAL", new NormalDistribution() },
		{ "UNIFORM", new UniformDistribution() },
	};
}
