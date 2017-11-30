/*27.01.2017 21:26
Структура для хранения статистики транзакта,а именно времени появления в сети и времени 
выхода из сети.
*/
#pragma once
namespace Statistic
{
	//Статистика транзакта. Хранит такт появления и выхода из сети
	struct TransactStatistic
	{
		int Start;
		int Finish;

		TransactStatistic()
		{
			Start = 0;
			Finish = 0;
		}

	};
}