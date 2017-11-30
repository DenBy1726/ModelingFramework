/*
15.02.2017 19:13
��������� ��� ������ � �������. ����������� ����� �������� �������� ����� �� ����������.
������ � wstring ��� ����������� ������������� �������� unicode
*/
#include "Locale.h"
#include "FullStatistic.cpp"

namespace Report
{
	class IReport
	{
	public:
		std::wstring Name = L"Undefined";
		std::wstring Separator = L"----------";
		virtual std::wstring GetReport(const Statistic::FullStatistic&) = 0;
	};
}