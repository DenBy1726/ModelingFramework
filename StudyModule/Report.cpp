/*
15.02.2017 19:19
Класс для печати отчета по статистике в заданной локали в строковую переменную
*/
#pragma once
#include "IReport.h"
#include "Locale.h"
#include "Function.cpp"
namespace Report
{
	class Report : public IReport
	{
	public:
		std::wstring ReportText;
		Locale Loc;
		/*	Отчет:
		-------------------- -
		Количество транзактов :
		Время :
		Модельное время :
		Количество устройств :
		--------------------
		для каждого :
		--------------------
		Номер :
		Загрузка канала(ro) :
		Средняя длинна очереди(l) :
		Среднее число транзактов канале(m) :
		Среднее время пребывания в очереди(w) :
		Среднее время работы(u) :
		------------------ -
		Системные характеристики :
		Средняя длинна очереди(l) :
		Среднее число транзактов канале(m) :
		Среднее время пребывания в очереди(w) :
		Среднее время работы(u) :
		*/

		Report(Locale* reportLocale)
		{
			setlocale(LC_ALL, "");
			Loc = *reportLocale;
		}

		virtual std::wstring GetReport(const Statistic::FullStatistic& state)
		{
			ReportText.clear();
			AddLine(Loc.Report, Name);
			Separate();
			AddLine(Loc.TransactAmount, state.ModelInfo.TransactAmount);
			AddLine(Loc.Time, state.ModelInfo.FinishTime - state.ModelInfo.StartTime);
			AddLine(Loc.ModelTime, state.ModelInfo.ModelTime);
			AddLine(Loc.DeviceAmount, state.LocalStatistic.size());
			Separate();
			for (int i = 0; i < state.LocalStatistic.size(); i++)
			{
				AddLine(Loc.Number, i + 1);
				AddLine(Loc.Ro, state.LocalStatistic[i].Probability);
				AddLine(Loc.L, state.LocalStatistic[i].Length);
				AddLine(Loc.M, state.LocalStatistic[i].TransactAmount);
				AddLine(Loc.W, state.LocalStatistic[i].WaitTime);
				AddLine(Loc.U, state.LocalStatistic[i].ProcessingTime);
				Separate();
			}
			AddLine(Loc.SystemCharacteristic);
			AddLine(Loc.L, state.GlobalStatistic.Length);
			AddLine(Loc.M, state.GlobalStatistic.TransactAmount);
			AddLine(Loc.W, state.GlobalStatistic.WaitTime);
			AddLine(Loc.U, state.GlobalStatistic.ProcessingTime);
			return ReportText;
		}

	protected:

	


		void Separate()
		{
			ReportText += this->Separator;
			ReportText += L"\n";
		}

		void NewLine()
		{
			ReportText += L"\n";
		}

		void AddLine(std::wstring line)
		{
			ReportText += line;
			ReportText += L" : ";
			NewLine();
		}

		template<class T>
		void AddLine(std::wstring line, T param)
		{
			ReportText += line;
			ReportText += L" : ";
			ReportText += Function::ToWString(param);
			NewLine();
		}

	};
}