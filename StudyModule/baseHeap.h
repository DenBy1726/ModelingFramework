/*
18.01.2017 14:22
������� ������ ���
�������� ������� � ��������� ��� ��������� ���������� ���
								�������:
	�������� ����� ���������
	��������� ��� �� ������ ���������
	���������, ��� �� ������ ����(��� 2-3 ����)
	���������, ��� �� ���� ���������������(��� 2-3 ����)
								���������:
	������� ����������� ��������
	������� ��������� ������ ��� min ���
								������������:
	�������������� ���
								��������� �� �������
	������� ��������� ������

18.01.2017 14:46
								����������:
	������� ������ ������� ����� �����������
	������� ����������� ���������
*/
#pragma once
namespace Heap
{

#define Ptr(a) (unsigned int)a
	//��� ������������ ������
#define DELETED 0xfeeefeee
	//������ ��� �����������?
#define IsBadPtr(a) Ptr(a) == DELETED
	//����� ��� �� �����������?
#define IsGoodPtr(a) Ptr(a) != DELETED
	//������� ��� �� ������?
#define IsNotDeleted(a) Ptr(a->left) != DELETED && Ptr(a->right) != DELETED && Ptr(a->partner) !=DELETED && Ptr(a->parent)!=DELETED && Ptr(a->child)!= DELETED
	//������� ��� ������?
#define IsBadPtrFull(a) Ptr(a->left) != DELETED || Ptr(a->right) != DELETED || Ptr(a->partner) !=DELETED || Ptr(a->parent)!=DELETED || Ptr(a->child)!= DELETED
	//������� ���������������?
#define IsNotNull(a) Ptr(a->left) != NULL && Ptr(a->right) != NULL && Ptr(a->partner) !=NULL && Ptr(a->parent)!=NULL && Ptr(a->child)!= NULL

	//������� ��������� ������������� ������
	typedef bool(*compFunc)(int, int);

	bool KeyMin(int, int);

	template<class T>
	T hmin(const T& a, const T& b);

	template<class T>
	void swap(T& a, T& b)
	{
		T temp = a;
		a = b;
		b = temp;
	}


	template<class T>
	T max(const T& a, const T&b)
	{
		return a > b ? a : b;
	}


	enum heapId
	{
		binary, fib, heap23
	};



}