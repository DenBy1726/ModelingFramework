#pragma once
#include "DistributionBuilder.h"
#include "Distribution.cpp"
namespace Random
{
	


	IDistribution<double>* DistributionBuilder::Create(const std::string& DT, const std::vector<double>& arg)
	{
		return Distribution[DT]->Clone(arg);
	}

	IDistribution<double>* DistributionBuilder::Create(const Descriptors::DistributionDescriptor& DT)
	{
		return Create(DT.Type, DT.ArgList);
	}

	std::map<std::string, IDistribution<double>* > DistributionBuilder::Distribution
	{
		{ "BernoulliDistribution" , new BernoulliDistribution() },
		{ "BinomialDistribution" , new BinomialDistribution() },
		{ "ExponentialDistribution", new ExponentialDistribution() },
		{ "IntExponentialDistribution", new IntExponentialDistribution() },
		{ "NormalDistribution", new NormalDistribution() },
		{ "UniformDistribution", new UniformDistribution() },
		{ "UniformRealDistribution", new UniformRealDistribution() }
	};
}
