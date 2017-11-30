/*
18.01.2017 15:42
2-3 ����.
���� ���������� �������� �� ������������� ���� � ���� ������� "���" ������ ������� �������
���� ���� 2-3 ������. ������ �� ������� �������� ��������� ����.
��������� �:
������� �.�. ����������� ���������� �� ������ � ������� 2-3 ���
Tadao Takaoka.   Theory of 2-3 Heaps. � Cocoon (1999).
*/
#pragma once
#include "baseHeap.h"
namespace Heap
{

#ifndef NODE_23
#define NODE_23

	//******************************************************************
	//*****************************�����-�����������********************
	//******************************************************************


	//***********************************************************************
	//************************���� ����(HeapNode) : ��������*****************
	//***********************************************************************

	//��������� ������� ���� �������� � ����.
	//�������� ��������� ��������� � ������
	//���������� �� ��������� � Heap � PriorityQueue
	template <class T>
	class HeapNode
	{
	protected:
		template<class T> friend class Heap;
		template<class T> friend class PriorityQueue;
		template <class T> friend void mergeNode(HeapNode<T>*& a, HeapNode<T>*& b, compFunc);
		//��������� ���������
		HeapNode* parent;
		HeapNode* partner;
		HeapNode* child;
		HeapNode* left;
		HeapNode* right;
		//��������� ������
		int degree;
		bool sl;
		//�������� ������
		int _key;
		T _value;
	public:
		//����������� ������� ���� � �������������� ���
		HeapNode(int k, T v) :left(NULL), child(NULL), right(NULL), _key(k), _value(v), degree(0), sl(false), parent(NULL), partner(NULL){};
		//����������� ������� ����,�������������� ������ ��������( ���� ����� ����)
		HeapNode(T v) :left(NULL), child(NULL), right(NULL), _key(0), _value(v), degree(0), sl(false), parent(NULL), partner(NULL){};
		//����������� �� ��������� ������� ������ ����
		HeapNode(){};
		~HeapNode(){}
		//���������� ��������� ����(��� ����). �������� ��������� ������ key()
		int priority()
		{
			return _key;
		}
		//���������� ��������� ����(��� ����)
		int key()
		{
			return _key;
		}
		//���������� ��������,�������� � ����
		T value()
		{
			return _value;
		}
		//���������� ������� ������(���������� ��� ������)
		int order()
		{
			return degree;
		}
		//���������� ��������� �� ������ ������
		HeapNode<T>* getLeft()
		{
			return left;
		}
		//���������� ��������� �� ����
		HeapNode<T>* getChild()
		{
			return child;
		}
		//���������� ��������� �� ������� ������
		HeapNode<T>* getRight()
		{
			return right;
		}
		//���������� ��������� �� ��������
		HeapNode<T>* getPartner()
		{
			return partner;
		}
		//���������� ��������� �� ��������
		HeapNode<T>* getParent()
		{
			return parent;
		}
		//���������� ������������ ����
		bool type()
		{
			return sl;
		}
		//��������� ���� � �������� ����
		void addChild(HeapNode<T>* new_child);
		//������ ������� ����� � �������� �����
		void swap(HeapNode<T>* root2);
		//�������� ���������
		bool operator<(HeapNode<T>* right)
		{
			return this->_key < right->_key;
		}

	};

	


	//*************************************************************
	//****************************���� ( Heap ) : ��������*********
	//*************************************************************

	/*

		����� ��� ������ � �����.
		�������� ������ ����� ( �� ���� ��������), � ��������� ����������
		���������� � HeapNode

		*/
	template <class T>
	class Heap
	{
	protected:
		template<class T> friend class HeapNode;
		template<class T> friend void mergeNode(HeapNode<T>*& a, HeapNode<T>*& b, compFunc);
		//������ �����

		HeapNode<T>** trees;

		//��������� ����������

		size_t _size;
		int _max;
		compFunc compare;
		typedef void(*walkFunc)(HeapNode<T>*);
	public:
		//������ ��������� ��� �� ������ ����
		static bool isNotDeleted(HeapNode<T>* it);
		//����������� ������������� ������� ��������� � ������������ ���������� �������� � ����
		Heap(compFunc call = KeyMin, int max = 16);

		~Heap()
		{
			walk(destructHeap);
		}

		//���������� ��������� �� ������ ���������
		compFunc getFunc() const
		{
			return compare;
		}

		//�������� �� �������
		bool isEmpty() const
		{
			return (_size == 0);
		}

		//���������� ������������ ������� ����
		int getMaxDegree() const
		{
			return _max;
		}

		//����� �� ���� � ���������� ������� action ��� ������ �������
		void walk(walkFunc action);

		//���������� ���������� ��������� � ����
		size_t size() const
		{
			return _size;
		}

		//���������� �������������������� ���������� ��������� � ����
		size_t maxSize() const
		{
			return (pow(3, _max + 1) - 1);
		}
		//���������� ������ ������� x
		HeapNode<T>* getTree(int x) const;

		//������� ���� � �������� ��� � ����
		HeapNode<T>* insert(int k, T v);

		//�������� ���� � ����
		void insert(HeapNode<T>* list);

		//���������� ������������ �������
		HeapNode<T>* priority();

		//������� �� ���� ������������ ������� � ���������� ���������� �� ���� �������
		//��������: � ������ ������ ���� �� ��������� �������������,
		//����� ������������ �� ���� ��������������
		HeapNode<T>* extract();

		//������� �� ���� ������������ ������� � ���������� �������� ����� ����������� ��������
		int extractKey();

		//������� �� ���� ������������ ������� � ���������� ��������,�������� � ���������� ��������
		T extractValue();

		//������� ������� ���� � ����� sum(� ���������� � sum ����� ������ ����)
		void merge(Heap& sum);

		///�������� �������� �����
		///����� �����������, � ����� ������������ �� ��� �������������, ��� ��� � 
		///����������� ���������� ������ � ��� ������ ��������� ������
		///��������� ����������� "������� �����������"
		///� ���������� ���� ����������� �� ����������� ���������, � ��� ���������� 
		///�������� ����� � ��������� ������� �������� �� ����������� ��������, � 
		///��������� �� ���.
		void editKey(HeapNode<T>* node, int new_key, T new_val);

	protected:
		//��������������� ������� ��� ������ �� ����
		//��������� ����� �� �������
		void walkNeighbor(HeapNode<T>*& it, walkFunc action);

		//������� ������� ������ �� ����
		void walkBase(HeapNode<T>*& it, HeapNode<T>*& prev, walkFunc action);


	};

	


	//*************************************************************
	//***������� � ����������� ( PriorityQueue ) : ��������********
	//*************************************************************

	/*
		����� ��������� �������������� ������ � ����� Heap ��� ��������� ������
		����������� ������� ������� ����������� ������ list
		������ � ������� HeapNode
		*/
	template <class T> class PriorityQueue : public Heap<T>
	{
	public:
		//������� ��������
		typedef bool(*unarPred)(HeapNode<T>*);

		template<class T> friend class HeapNode;

		//����������� �� ���������
		PriorityQueue<T>(compFunc call = keyMin, int max = 16);

		PriorityQueue(const PriorityQueue<T>&);
		void operator=(const PriorityQueue<T>&);

		//������� ����
		void clear();

		//�������� �� ���� x ��� �����
		void copy(const PriorityQueue<T>& x);

		
#ifdef _LIST_
		//�������� ��� ����������� ������ �� �����
		typedef void(*copyFunc)(HeapNode<T>*, std::list<std::pair< int , T > >&);

		//�������� ��� ����������� �����
		typedef void(*copyNodes)(HeapNode<T>*, std::list < HeapNode< T>* >&);

		//����������� � ���������� listData - ������ ���<����, ��������>
		PriorityQueue(std::list<std::pair<int, T> > listData, compFunc call = keyMin, int max = 16);

		//����������� � ���������� listData - ������ ������
		PriorityQueue(std::list<int> listData, compFunc call = keyMin, int max = 16);

		//������� ����� ������� � ����������� �� ��������� ������ , ������� � ����� ��� �����,
		//������� �������� �� �������
		//�������� ��������� : bool f(HeapNode<T>)
		PriorityQueue<T> createHeapIf(unarPred);

		//���������� ��� ����� ���� � ������� ������ <����, ��������>
		std::list<std::pair<int,T> > getAllData() const;

		//���������� ��� ����� ���� � ������� ������ �����.
		std::list<HeapNode<T>* > getAllNode();

	
	protected:
		void _walkCopy(copyFunc action,std::list<std::pair<int,T> >&) const;
		void _walkCopy(copyNodes action, std::list<HeapNode<T>* >&) const;
		//��������������� ������� ��� ������ �� ����
		//��������� ����� �� �������
		void _walkNeighborCopy(HeapNode<T>*& it, copyFunc action, std::list<std::pair<int, T> >&) const;
		void _walkNeighborCopy(HeapNode<T>*& it, copyNodes action, std::list<HeapNode<T>* >&) const;
		//������� ������� ������ �� ����
		void _walkBaseCopy(HeapNode<T>*& it, HeapNode<T>*& prev, copyFunc action, std::list<std::pair<int , T> >&) const;
		void _walkBaseCopy(HeapNode<T>*& it, HeapNode<T>*& prev, copyNodes action, std::list<HeapNode<T>* >&) const;

#endif
	};


#endif

}