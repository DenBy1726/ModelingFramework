/*27.01.2017 22:07
�������� ������ �� ����� �������. � ������ ������������� �������, ��������, �������
����������� ������� � ����� ������������� �������.
*/
#pragma once
#include "Time.h"
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
		Event(const EventId& id, Transact* sender, int time);

		Event();
		static Event* CreateFreeEvent(Transact* t,int time);

		~Event();
	};
}