/*27.01.2017 21:19
Статичтическая информация о изменеии состояния канала. Содержит данные о времени
смены состояния,флаг занятости и длинну очереди в данное время.
*/
#pragma once
namespace Statistic
{
	//одна запись в статистике состояний канала
	struct StateStatisticUnit
	{
		int Time;
		int Busy;
		int QueueLength;
		StateStatisticUnit()
		{
			Time = 0;
			Busy = 0;
			QueueLength = 0;
		}

		bool operator<(const StateStatisticUnit& s)const
		{
			return Time < s.Time;
		}
			
	};
}