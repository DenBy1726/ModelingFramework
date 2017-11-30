/*
18.01.2017 15:42
2-3 куча.
Идея реализации основана на представлении кучи в виде массива "куч" Каждый элемент массива
граф типа 2-3 дерево. Каждый из которых обладает свойством кучи.
Подробнее в:
Наливко Д.В. Оптимазация алгоритмов на графах с помощью 2-3 куч
Tadao Takaoka.   Theory of 2-3 Heaps. — Cocoon (1999).
*/
#pragma once
#include "baseHeap.h"
namespace Heap
{

#ifndef NODE_23
#define NODE_23

	//******************************************************************
	//*****************************МАКРО-ОПРЕДЕЛЕНИЯ********************
	//******************************************************************


	//***********************************************************************
	//************************Узел Кучи(HeapNode) : Прототип*****************
	//***********************************************************************

	//Позволяет хранить одно значение в куче.
	//Содержит служебные указатели и данные
	//Дружелюбен по отношению к Heap и PriorityQueue
	template <class T>
	class HeapNode
	{
	protected:
		template<class T> friend class Heap;
		template<class T> friend class PriorityQueue;
		template <class T> friend void mergeNode(HeapNode<T>*& a, HeapNode<T>*& b, compFunc);
		//Служебные указатели
		HeapNode* parent;
		HeapNode* partner;
		HeapNode* child;
		HeapNode* left;
		HeapNode* right;
		//Служебные данные
		int degree;
		bool sl;
		//Хранимые данные
		int _key;
		T _value;
	public:
		//конструктор создает узел и инициализирует его
		HeapNode(int k, T v) :left(NULL), child(NULL), right(NULL), _key(k), _value(v), degree(0), sl(false), parent(NULL), partner(NULL){};
		//конструктор создает узел,инициализирует только значение( ключ равен нулю)
		HeapNode(T v) :left(NULL), child(NULL), right(NULL), _key(0), _value(v), degree(0), sl(false), parent(NULL), partner(NULL){};
		//Конструктор по умолчанию создает пустой узел
		HeapNode(){};
		~HeapNode(){}
		//возвращает приоритет узла(его ключ). Является синонимом метода key()
		int priority()
		{
			return _key;
		}
		//возвращает приоритет узла(его ключ)
		int key()
		{
			return _key;
		}
		//возвращает значение,хранимое в узле
		T value()
		{
			return _value;
		}
		//возвращает степень дерева(фактически его высота)
		int order()
		{
			return degree;
		}
		//возвращает указатель на левого соседа
		HeapNode<T>* getLeft()
		{
			return left;
		}
		//возвращает указатель на сына
		HeapNode<T>* getChild()
		{
			return child;
		}
		//возвращает указатель на правого соседа
		HeapNode<T>* getRight()
		{
			return right;
		}
		//возвращает указатель на партнера
		HeapNode<T>* getPartner()
		{
			return partner;
		}
		//возвращает указатель на родителя
		HeapNode<T>* getParent()
		{
			return parent;
		}
		//возвращает насыщенность узла
		bool type()
		{
			return sl;
		}
		//Вставляет сына к текущему узлу
		void addChild(HeapNode<T>* new_child);
		//меняет местами ключи и значения узлов
		void swap(HeapNode<T>* root2);
		//оператор сравнения
		bool operator<(HeapNode<T>* right)
		{
			return this->_key < right->_key;
		}

	};

	


	//*************************************************************
	//****************************Куча ( Heap ) : Прототип*********
	//*************************************************************

	/*

		Класс для работы с кучей.
		Содержит массив узлов ( по сути деревьев), и служебную информацию
		Дружелюбен с HeapNode

		*/
	template <class T>
	class Heap
	{
	protected:
		template<class T> friend class HeapNode;
		template<class T> friend void mergeNode(HeapNode<T>*& a, HeapNode<T>*& b, compFunc);
		//массив узлов

		HeapNode<T>** trees;

		//служебная информация

		size_t _size;
		int _max;
		compFunc compare;
		typedef void(*walkFunc)(HeapNode<T>*);
	public:
		//Фунция проверяет был ли удален узел
		static bool isNotDeleted(HeapNode<T>* it);
		//конструктор устанавливает функцию сравнения и максимальное количество деревьев в куче
		Heap(compFunc call = KeyMin, int max = 16);

		~Heap()
		{
			walk(destructHeap);
		}

		//возвращает указатель на фунцию сравнения
		compFunc getFunc() const
		{
			return compare;
		}

		//проверка на пустоту
		bool isEmpty() const
		{
			return (_size == 0);
		}

		//возвращает максимальную степень кучи
		int getMaxDegree() const
		{
			return _max;
		}

		//обход по куче и выполнение функции action для каждой вершины
		void walk(walkFunc action);

		//возвращает количество элементов в куче
		size_t size() const
		{
			return _size;
		}

		//возвращает максимальновозможное количество элементов в куче
		size_t maxSize() const
		{
			return (pow(3, _max + 1) - 1);
		}
		//возвращает дерево степени x
		HeapNode<T>* getTree(int x) const;

		//создать узел и вставить его в кучу
		HeapNode<T>* insert(int k, T v);

		//вставить узел в кучу
		void insert(HeapNode<T>* list);

		//возвращает приоритетный элемент
		HeapNode<T>* priority();

		//удаляет из кучи приоритетный элемент и возвращает вырезанный из кучи элемент
		//Внимание: в данном случае узел не удалиться автоматически,
		//нужно позаботиттся об этом свмостоятельно
		HeapNode<T>* extract();

		//удаляет из кучи приоритетный элемент и возвращает значение ключа вырезанного элемента
		int extractKey();

		//удаляет из кучи приоритетный элемент и возвращает значение,хранимое в вырезанном элемента
		T extractValue();

		//сливает текущую кучу с кучей sum(в результате в sum будет пустая куча)
		void merge(Heap& sum);

		///изменяет значение ключа
		///метод небезопасен, и лучше воздержаться от его использования, так как с 
		///увеличением количества вершин и при частом изменении ключей
		///возникает вероятность "ложного просеивания"
		///в результате куча отклоняется от корректного состояния, и при извлечении 
		///минимума может в еденичных случаях выдавать не приритетное значение, а 
		///следующее за ним.
		void editKey(HeapNode<T>* node, int new_key, T new_val);

	protected:
		//вспомогательная функция для обхода по куче
		//совершает обход по соседям
		void walkNeighbor(HeapNode<T>*& it, walkFunc action);

		//базовая функция обхода по куче
		void walkBase(HeapNode<T>*& it, HeapNode<T>*& prev, walkFunc action);


	};

	


	//*************************************************************
	//***Очередь с приоритетом ( PriorityQueue ) : Прототип********
	//*************************************************************

	/*
		Класс добавляет дополнительные методы к класу Heap для упращения работы
		Большинство методов требуют подключения класса list
		Дружит с классом HeapNode
		*/
	template <class T> class PriorityQueue : public Heap<T>
	{
	public:
		//унарный предикат
		typedef bool(*unarPred)(HeapNode<T>*);

		template<class T> friend class HeapNode;

		//Конструктор по умолчанию
		PriorityQueue<T>(compFunc call = keyMin, int max = 16);

		PriorityQueue(const PriorityQueue<T>&);
		void operator=(const PriorityQueue<T>&);

		//очищает кучу
		void clear();

		//копирует из кучи x все ключи
		void copy(const PriorityQueue<T>& x);

		
#ifdef _LIST_
		//предикат для копирования данных из узлов
		typedef void(*copyFunc)(HeapNode<T>*, std::list<std::pair< int , T > >&);

		//Предикат для копирования узлов
		typedef void(*copyNodes)(HeapNode<T>*, std::list < HeapNode< T>* >&);

		//Конструктор с параметром listData - список пар<ключ, значение>
		PriorityQueue(std::list<std::pair<int, T> > listData, compFunc call = keyMin, int max = 16);

		//Конструктор с параметром listData - список ключей
		PriorityQueue(std::list<int> listData, compFunc call = keyMin, int max = 16);

		//создает новую очередь с приоритетом на основании старой , помещая в новую все ключи,
		//которые подходят по условию
		//прототип предиката : bool f(HeapNode<T>)
		PriorityQueue<T> createHeapIf(unarPred);

		//возвращает все ключи кучи в формате списка <ключ, значение>
		std::list<std::pair<int,T> > getAllData() const;

		//возвращает все ключи кучи в формате списка узлов.
		std::list<HeapNode<T>* > getAllNode();

	
	protected:
		void _walkCopy(copyFunc action,std::list<std::pair<int,T> >&) const;
		void _walkCopy(copyNodes action, std::list<HeapNode<T>* >&) const;
		//вспомогательная функция для обхода по куче
		//совершает обход по соседям
		void _walkNeighborCopy(HeapNode<T>*& it, copyFunc action, std::list<std::pair<int, T> >&) const;
		void _walkNeighborCopy(HeapNode<T>*& it, copyNodes action, std::list<HeapNode<T>* >&) const;
		//базовая функция обхода по куче
		void _walkBaseCopy(HeapNode<T>*& it, HeapNode<T>*& prev, copyFunc action, std::list<std::pair<int , T> >&) const;
		void _walkBaseCopy(HeapNode<T>*& it, HeapNode<T>*& prev, copyNodes action, std::list<HeapNode<T>* >&) const;

#endif
	};


#endif

}