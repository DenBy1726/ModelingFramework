/*
15.02.2017
Реализация русской локали
*/
#pragma once
#include "Locale.h"
namespace Report
{
	struct RussianLocale : public Locale
	{
	public:
		RussianLocale()
		{
			this->DeviceAmount = L"Количество узлов";
			this->L = L"Средняя длинна очереди (l)";
			this->M = L"Среднее число транзактов канале (m)";
			this->ModelTime = L"Модельное Время";
			this->Number = L"Номер";
			this->Report = L"Отчет";
			this->Ro = L"Загрузка канала (ро)";
			this->SystemCharacteristic = L"Системные характеристики";
			this->Time = L"Время";
			this->TransactAmount = L"Количество транзактов";
			this->U = L"Среднее время пребывания в устройстве (u)";
			this->W = L"Среднее время пребывания в очереди (w)";
		}
	};
}
