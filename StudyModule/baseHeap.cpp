/*
18.01.2017 14:23
���������� ��������� ���������� ������� �� �������� ������ ���
							�������:
	��������� ������������
	��������� ������ ��� min ���

*/

#include "baseHeap.h"
#pragma once
namespace Heap
{
	//������� ����������� ��������
	template <class T>
	T hmin(const T& a, const T& b){
		return a > b ? b : a;
	}

	bool KeyMin(int a, int b)
	{
		return a < b ? true : false;
	}

}
