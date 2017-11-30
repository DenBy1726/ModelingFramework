/*
21.01.2017 23:51
Реализация генератора случайных чисел по алгоритму mersenne twister.
Реализация взята из std::mt19937
*/
#pragma once
#include "IRandom.h"
#include <chrono>
#include <iostream>
#include <thread>
#include <random>

namespace Random
{

	class Random : public IRandom
	{
		std::mt19937 _gen;
	public:

		Random()
		{
			std::this_thread::sleep_for(std::chrono::milliseconds(1));
			auto now = std::chrono::system_clock::now();
			auto now_ms = std::chrono::time_point_cast<std::chrono::milliseconds>(now);
			auto epoch = now_ms.time_since_epoch();
			auto value = std::chrono::duration_cast<std::chrono::milliseconds>(epoch);
			long duration = value.count();
			_gen.seed(duration);
		}

		int Next(int min, int max)
		{
			return _gen() % (max - min) + min;
		}

		double NextDouble()
		{
			return double(_gen()) / double(_gen.max());
		}

		//возвращает ссылку на Движок генератора
		std::mt19937& Engine()
		{
			return _gen;
		}
	};
}