/*
12.04.2017 21:21 
��������, ������� ������� ������������ ������
*/
#pragma once
#include "Transact.h"
#include "Time.h"
namespace Model
{
	class Transact;
	class Terminal
	{
		
		int count;

	public:

		Terminal();

		//�������� ���������� �������� ������
		int GetCount();

		//�������� ��� ������ �������� ����� � �������
		void Terminate(Transact* t, int time);
		void Clear();
	};
}