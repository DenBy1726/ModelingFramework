/*27:01:2017 21:30
Статистика Модели. Содержит данные о колиестве транзактов, а также времени начала и 
конца моделирования.
*/
#pragma once
namespace Statistic
{

	struct ModelStatistic
	{
		int TransactAmount;
		int StartTime;
		int FinishTime;
		int ModelTime;

		ModelStatistic(const ModelStatistic& state)
		{
			TransactAmount = state.TransactAmount;
			StartTime = state.StartTime;
			FinishTime = state.FinishTime;
			ModelTime = state.ModelTime;
		}

		ModelStatistic()
		{
			TransactAmount = 0;
			StartTime = 0;
			FinishTime = 0;
			ModelTime = 0;
		}

		void Clear()
		{
			TransactAmount = 0;
			StartTime = 0;
			FinishTime = 0;
			ModelTime = 0;
		}
	};
}