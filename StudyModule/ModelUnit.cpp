/*27:01:2017 21:39
Еденица статистики для вывода. Содержит по одному значению каждой интересующей
величины, которая расчитввается на основании накопленных моделью данных.
*/
#pragma once
namespace Statistic
{

	struct ModelUnit
	{
		//вероятность что канал занят(коэффициент загрузки) po
		double Probability;
		//средняя длинна очереди l
		double Length;
		//среднее время ожидания. w
		double WaitTime;
		// среднее количество транзактов в узле m
		double TransactAmount;
		//среднее время пребывания в узле  u
		double ProcessingTime;
		//коэффициент передачи лямбда
		double TransferCoefficient;

		ModelUnit()
		{
			Probability = 0;
			Length = 0;
			WaitTime = 0;
			TransactAmount = 0;
			ProcessingTime = 0;
			TransferCoefficient = 0;
		}

		ModelUnit(const ModelUnit& state)
		{
			Probability = state.Probability;
			Length = state.Length;
			WaitTime = state.WaitTime;
			TransactAmount = state.TransactAmount;
			ProcessingTime = state.ProcessingTime;
			TransferCoefficient = state.TransferCoefficient;
		}

		void Clear()
		{
			Probability = 0;
			Length = 0;
			WaitTime = 0;
			TransactAmount = 0;
			ProcessingTime = 0;
			TransferCoefficient = 0;
		}

	};
}



