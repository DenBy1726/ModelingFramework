/*27:01:2017 21:39
������� ���������� ��� ������. �������� �� ������ �������� ������ ������������
��������, ������� ������������� �� ��������� ����������� ������� ������.
*/
#pragma once
namespace Statistic
{

	struct ModelUnit
	{
		//����������� ��� ����� �����(����������� ��������) po
		double Probability;
		//������� ������ ������� l
		double Length;
		//������� ����� ��������. w
		double WaitTime;
		// ������� ���������� ���������� � ���� m
		double TransactAmount;
		//������� ����� ���������� � ����  u
		double ProcessingTime;
		//����������� �������� ������
		double TransferCoefficient;

		ModelUnit()
		{
			Probability = 0;
			Length = 0;
			WaitTime = 0;
			TransactAmount = 0;
			ProcessingTime = 0;
			TransferCoefficient = 0;
		}

		ModelUnit(const ModelUnit& state)
		{
			Probability = state.Probability;
			Length = state.Length;
			WaitTime = state.WaitTime;
			TransactAmount = state.TransactAmount;
			ProcessingTime = state.ProcessingTime;
			TransferCoefficient = state.TransferCoefficient;
		}

		void Clear()
		{
			Probability = 0;
			Length = 0;
			WaitTime = 0;
			TransactAmount = 0;
			ProcessingTime = 0;
			TransferCoefficient = 0;
		}

	};
}



