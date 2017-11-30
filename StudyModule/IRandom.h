/*
21.07.2017 23:32
Интерфейс для генератора чисел
Согласно интерфейсу IRandom генератор должен уметь:
1)Генерировать целые числа из заданного диапазона
2)Генерировать вещественные числа из диапазона [0,1]
*/
#pragma once
namespace Random
{

	class IRandom
	{
	public:
		virtual int Next(int min, int max) = 0;
		virtual double NextDouble() = 0;
	};
}