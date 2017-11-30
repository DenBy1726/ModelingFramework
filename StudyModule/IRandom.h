/*
21.07.2017 23:32
��������� ��� ���������� �����
�������� ���������� IRandom ��������� ������ �����:
1)������������ ����� ����� �� ��������� ���������
2)������������ ������������ ����� �� ��������� [0,1]
*/
#pragma once
namespace Random
{

	class IRandom
	{
	public:
		virtual int Next(int min, int max) = 0;
		virtual double NextDouble() = 0;
	};
}