/*
15.02.2017
���������� ������� ������
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
			this->DeviceAmount = L"���������� �����";
			this->L = L"������� ������ ������� (l)";
			this->M = L"������� ����� ���������� ������ (m)";
			this->ModelTime = L"��������� �����";
			this->Number = L"�����";
			this->Report = L"�����";
			this->Ro = L"�������� ������ (��)";
			this->SystemCharacteristic = L"��������� ��������������";
			this->Time = L"�����";
			this->TransactAmount = L"���������� ����������";
			this->U = L"������� ����� ���������� � ���������� (u)";
			this->W = L"������� ����� ���������� � ������� (w)";
		}
	};
}
