/*
21.01.2017 23:59
»нтерфейс генератора с распределением.—одержит в себе указатель на генератор.¬ качестве параметра выступает тип 
выходных значений.
—огласно данному интерфейсу генератор должен уметь:
1)√енерировать следующее число
2)¬озвращать генератор,который используетс€.
///////////////////
18.02.2017 14:37
3)ѕолучить тип распределени€
4)ѕолучить аргумента распределени€
–аспределени€ получили возможность возвращать свое состо€ние
04.04.2017 22:25
5)—делать копию распределени€ с новыми аргументами
*/
#pragma once
#include "Random.cpp"
#include <map>
namespace Random
{


	template<class RetVal>
	class IDistribution
	{
	protected:
		Random _gen;


	public:
		IDistribution(const Random& gen = Random())
		{
			_gen = gen;
		}
		virtual RetVal Next() = 0;
		virtual Random Generator()
		{
			return _gen;
		}
		virtual std::string Type() = 0;
		virtual std::vector<double> Argument() = 0;
		virtual IDistribution<double>* Clone(const std::vector<double>&) = 0;

	};

	typedef IDistribution<double> DDistribution;


}