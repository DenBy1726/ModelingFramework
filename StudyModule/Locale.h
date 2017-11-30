/*
15.02.2017 18:47
Структура для работы с локалью. Необходим для возможности поддержки печати отчетов на разных языках
*/
#pragma once
#include <string>
namespace Report
{
	struct Locale
	{
		std::wstring Report;
		std::wstring TransactAmount;
		std::wstring Time;
		std::wstring ModelTime;
		std::wstring DeviceAmount;
		std::wstring Number;
		std::wstring Ro;
		std::wstring L;
		std::wstring M;
		std::wstring U;
		std::wstring W;
		std::wstring SystemCharacteristic;
		
		Locale()
		{

		}

		Locale(const Locale& copy)
		{
			Report = copy.Report;
			TransactAmount = copy.TransactAmount;
			Time = copy.Time;
			ModelTime = copy.ModelTime;
			DeviceAmount = copy.DeviceAmount;
			Number = copy.Number;
			Ro = copy.Ro;
			L = copy.L;
			M = copy.M;
			U = copy.U;
			W = copy.W;
			SystemCharacteristic = copy.SystemCharacteristic;
		}
	};
}