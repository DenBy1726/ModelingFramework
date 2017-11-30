/*
18.01.2017 14:25
Описание Бинарной кучи
Содержит описание узла Бинарной кучи и самой Бинарной кучи.
Куча реализована на векторе. Идея реализации основана на представлении кучи в виде массива и манипулирования
с кучей по средством обмена данными между ячейками,и стремлении соблюдать свойство кучи.
Подробнее смотрите в Кормене
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
		//конструктор по умолчанию
		BinaryHeap(const compFunc& comp = KeyMin)
		{
			compare = comp;
		}
		BinaryHeap(const BinaryHeap<T>&);
		BinaryHeap(const std::vector<BinaryNode<T> >& vals, const compFunc& comp = baseComp);
		BinaryHeap(const std::list<int> & vals, const compFunc& comp = baseComp);
		//изменяет запись с индексом index на (newkey(), new_val)
		void editKey(int index, const int& newkey, const T& new_val);
		//извлекает из кучи приоритетный элемент и возвращает его
		BinaryNode<T> extract();
		//извлекает из кучи приоритетный элемент и возвращает только ключ
		int extractKey();
		//извлекает из кучи приоритетный элемент и возвращает только значение
		T extractValue();
		//возвращает предикат функции приоритета
		compFunc getFunc()
		{
			return compare;
		}
		//возвращает запись под индексом index
		BinaryNode<T> getData(const int& index);
		//возвращает список всех записей
		std::vector<BinaryNode<T> > getData();
		//вставляет запись (key,value)
		//возвращает индекс вставленной записи
		int insert(const int& key, const T& value);
		//сливает кучу с кучей sum
		//sum станет пустой
		void merge(BinaryHeap<T>& sum);
		//сливает кучу с кучей sum
		void copy(BinaryHeap<T>& sum);
		//возвращает приоритетный элемент
		BinaryNode<T> priority();
		//возвращает размер кучи
		size_t size();
		//выполнить функцию для каждого элемента кучи
		void walk(const walkFunc& action);
		//очищает кучу
		void clear();
		//создает новую кучу на оснавании текущей из ключей, которые
		//подходят по предикату
		BinaryHeap createHeapIf(const unarPred&);
		bool isEmpty();
		BinaryNode<T> operator[](const int& i);
	};


#endif
}