#pragma once
#include "Terminal.h"

namespace Model
{
	Terminal::Terminal()
	{
		count = 0;
	}
	//�������� ���������� �������� ������
	int Terminal::GetCount()
	{
		return count;
	}

	//�������� ��� ������ �������� ����� � �������
	void Terminal::Terminate(Transact* t, int time)
	{
		t->OnStop(time);
		count++;
	}

	void Terminal::Clear()
	{
		count = 0;
	}
}