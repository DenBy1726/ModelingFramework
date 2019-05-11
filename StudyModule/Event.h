/*27.01.2017 22:07
�������� ������ �� ����� �������. � ������ ������������� �������, ��������, �������
����������� ������� � ����� ������������� �������.
*/
#pragma once
#include "Transact.h"
#include "EventId.h"


namespace Model
{
	class Transact;
	struct Event
	{
		EventId Id;
		//Event ������ ������ ���������,� �� ������� ���, � �������������
		//�� ������ �������� �� �����������.
		Transact* Sender;
		int Time;
		Event(const EventId& id, Transact* sender, int time)
		{
			this->Id = id;
			this->Sender = sender;
			this->Time = time;
		}

		~Event()
		{
			Time = 0;
		}
	};
}