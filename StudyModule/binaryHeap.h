/*
18.01.2017 14:25
�������� �������� ����
�������� �������� ���� �������� ���� � ����� �������� ����.
���� ����������� �� �������. ���� ���������� �������� �� ������������� ���� � ���� ������� � ���������������
� ����� �� ��������� ������ ������� ����� ��������,� ���������� ��������� �������� ����.
��������� �������� � �������
*/
#include "baseHeap.h"
#include <vector>
#include <list>
namespace Heap
{

#ifndef _BINARY_HEAP
#define _BINARY_HEAP


#define _leftChild_(i) ((2*i) + 1)
#define _rightChild_(i) ((2*i) + 2)
#define _parent_(i) ((i-1)/2)

	template <class T>
	struct BinaryNode
	{
		BinaryNode(int f = 0, T s = T()) : _key(f), _value(s){};
		BinaryNode(const BinaryNode& c) : _key(c._key), _value(c._value){};
		int _key;
		T _value;
		int key()
		{
			return _key;
		}
		T value()
		{
			return _value;
		}
	};
	template <class T>
	class BinaryHeap
	{
		std::vector<BinaryNode<T> > data;
		compFunc compare;
		typedef void(*walkFunc)(const BinaryNode<T>&);
		typedef bool(*unarPred)(BinaryNode<T>);
		void heapify(int i);
	public:
		//����������� �� ���������
		BinaryHeap(const compFunc& comp = KeyMin)
		{
			compare = comp;
		}
		BinaryHeap(const BinaryHeap<T>&);
		BinaryHeap(const std::vector<BinaryNode<T> >& vals, const compFunc& comp = baseComp);
		BinaryHeap(const std::list<int> & vals, const compFunc& comp = baseComp);
		//�������� ������ � �������� index �� (newkey(), new_val)
		void editKey(int index, const int& newkey, const T& new_val);
		//��������� �� ���� ������������ ������� � ���������� ���
		BinaryNode<T> extract();
		//��������� �� ���� ������������ ������� � ���������� ������ ����
		int extractKey();
		//��������� �� ���� ������������ ������� � ���������� ������ ��������
		T extractValue();
		//���������� �������� ������� ����������
		compFunc getFunc()
		{
			return compare;
		}
		//���������� ������ ��� �������� index
		BinaryNode<T> getData(const int& index);
		//���������� ������ ���� �������
		std::vector<BinaryNode<T> > getData();
		//��������� ������ (key,value)
		//���������� ������ ����������� ������
		int insert(const int& key, const T& value);
		//������� ���� � ����� sum
		//sum ������ ������
		void merge(BinaryHeap<T>& sum);
		//������� ���� � ����� sum
		void copy(BinaryHeap<T>& sum);
		//���������� ������������ �������
		BinaryNode<T> priority();
		//���������� ������ ����
		size_t size();
		//��������� ������� ��� ������� �������� ����
		void walk(const walkFunc& action);
		//������� ����
		void clear();
		//������� ����� ���� �� ��������� ������� �� ������, �������
		//�������� �� ���������
		BinaryHeap createHeapIf(const unarPred&);
		bool isEmpty();
		BinaryNode<T> operator[](const int& i);
	};


#endif
}