#pragma once
#include "Terminal.h"

namespace Model
{
	Terminal::Terminal()
	{
		count = 0;
	}
	//получить количество вышедших заявок
	int Terminal::GetCount()
	{
		return count;
	}

	//пометить что данный транзакт вышел и системы
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