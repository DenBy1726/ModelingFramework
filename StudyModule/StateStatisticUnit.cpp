/*27.01.2017 21:19
�������������� ���������� � �������� ��������� ������. �������� ������ � �������
����� ���������,���� ��������� � ������ ������� � ������ �����.
*/
#pragma once
namespace Statistic
{
	//���� ������ � ���������� ��������� ������
	struct StateStatisticUnit
	{
		int Time;
		int Busy;
		int QueueLength;
		StateStatisticUnit()
		{
			Time = 0;
			Busy = 0;
			QueueLength = 0;
		}

		bool operator<(const StateStatisticUnit& s)const
		{
			return Time < s.Time;
		}
			
	};
}