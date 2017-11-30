/*
18.01.2017 14:23
Реализация описанных прототипов функция из базового модуля куч
							Функции:
	Опреденее минимального
	Сравнение ключей для min куч

*/

#include "baseHeap.h"
#pragma once
namespace Heap
{
	//Функция определения минимума
	template <class T>
	T hmin(const T& a, const T& b){
		return a > b ? b : a;
	}

	bool KeyMin(int a, int b)
	{
		return a < b ? true : false;
	}

}
