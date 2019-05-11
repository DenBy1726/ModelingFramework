/*
04.04.2017
Фабрика распределений.
Ранее была в distribution.cpp
Конструирует распределение по типу и аргументам
12.04.2017 10:02
теперь статический класс
*/
#pragma once
#include "IDistribution.h"
#include "Descriptors.cpp"
#include <map>
#include <string>

namespace Random
{
	
	static class DistributionBuilder
	{
	public:
		static std::map<std::string, IDistribution<double>* > Distribution;
		
		
		static IDistribution<double>* Create(const std::string& DT, const std::vector<double>& arg);
		//Создание генератора распределения из дескриптора
		static IDistribution<double>* Create(const Descriptors::DistributionDescriptor& DT);

	};
	typedef DistributionBuilder Distribution;
}