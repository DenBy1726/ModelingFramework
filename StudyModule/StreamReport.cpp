/*
15.02.2017
Реализация печать отчета в поток
*/
#pragma once
#include "Report.cpp"
#include <fstream>
#include "Locale.h"
#include <locale>
#include <codecvt>
#include <iostream>

namespace Report
{
	class StreamReport : public Report
	{
		std::wostream* binded_stream;
		std::locale utf8_locale;

	public:
		//допустимы потоки: wostream, wofstream
		StreamReport(std::wostream* stream, Locale* loc)
			:StreamReport(loc)
		{
			Bind(stream);
		}

		StreamReport(Locale* loc)
			:Report(loc)
		{
			utf8_locale = std::locale(std::locale(), new std::codecvt_utf8<wchar_t>());
		}

		//привязывает поток к классу
		void Bind(std::wostream* stream)
		{
			binded_stream = stream;			
		}

		void Dispose()
		{
			binded_stream = nullptr;
		}

		void Print(const Statistic::FullStatistic& state)
		{
			if (binded_stream == nullptr)
				throw("Класс не был связан с потоком");
			GetReport(state);
			binded_stream->write(ReportText.c_str(),ReportText.size());
			
		}





	};
}
