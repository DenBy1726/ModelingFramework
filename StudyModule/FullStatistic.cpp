/*27:01:2017 21:44
содержит статистику для вывода пользователю, которая вклюяает в себя информацию модели,
статистику модели и статистику каждого канала.
*/
#pragma once
#include <vector>
#include "ModelStatistic.cpp"
#include "ModelUnit.cpp"
namespace Statistic
{

	struct FullStatistic
	{
		ModelStatistic ModelInfo;
		ModelUnit GlobalStatistic;
		std::vector<ModelUnit> LocalStatistic;

		FullStatistic(const FullStatistic& state)
		{
			ModelInfo = state.ModelInfo;
			GlobalStatistic = state.GlobalStatistic;
			LocalStatistic = state.LocalStatistic;
		}

		FullStatistic()
		{

		}
	};
}