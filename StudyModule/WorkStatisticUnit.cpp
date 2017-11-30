/*27.01.2017 21:17
Статичтическая информация о поведении транзакта в канале. Содержит данные о времени
прихода, захвата канала и освобождения.
*/
#pragma once
namespace Statistic
{
	//запись в статистике работы канала
	struct WorkStatisticUnit
	{
		int Enter;
		int Start;
		int Release;
		WorkStatisticUnit()
		{
			Enter = 0;
			Start = 0;
			Release = 0;
		}
	};
}