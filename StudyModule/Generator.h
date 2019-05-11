/*
07.04.2017 19:37
����� ��� ��������� ������(����������)
*/
#pragma once
#include "Distribution.cpp"
#include "Transact.h"
#include "Descriptors.cpp"
namespace Model
{
	class Generator
	{
		Random::IDistribution<double>* Random;
		int time;
	public:
		Generator(const Descriptors::DistributionDescriptor& dis);
		Generator(Random::IDistribution<double>* rnd);
		Transact* CreateTransact();
	};
}