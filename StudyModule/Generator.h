#pragma once
#include "IDistribution.h"
#include "Transact.h"
namespace Model
{
	
	class Transact;
	class Generator
	{
		int time;
	public:
		Random::IDistribution<double>* Distribution;
		Transact* CreateTransact();
		Generator();
		Generator::Generator(Random::IDistribution<double>* rnd);
		void Clear();
		~Generator();
	};
}
