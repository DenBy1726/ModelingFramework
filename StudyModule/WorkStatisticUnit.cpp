/*27.01.2017 21:17
�������������� ���������� � ��������� ��������� � ������. �������� ������ � �������
�������, ������� ������ � ������������.
*/
#pragma once
namespace Statistic
{
	//������ � ���������� ������ ������
	struct WorkStatisticUnit
	{
		int Enter;
		int Start;
		int Release;
		WorkStatisticUnit()
		{
			Enter = 0;
			Start = 0;
			Release = 0;
		}
	};
}