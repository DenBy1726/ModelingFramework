/*
18.01.2017 14:22
Базовый модуль куч
Содержит макросы и прототипы для различных реализаций куч
								Макросы:
	Получить адрес указателя
	Проверить был ли удален указатель
	Проверить, был ли удален узел(для 2-3 кучи)
	Проверить, был ли узел инициализирован(для 2-3 кучи)
								Прототипы:
	Функция определения минимума
	Функция сравнения ключей для min куч
								Перечисления:
	Идентификаторы куч
								Указатели на функции
	Функция сравнения ключей

18.01.2017 14:46
								Реализации:
	Функция обмена данными между переменными
	Функция определения максимума
*/
#pragma once
namespace Heap
{

#define Ptr(a) (unsigned int)a
	//уже освобожденая память
#define DELETED 0xfeeefeee
	//память уже освобождена?
#define IsBadPtr(a) Ptr(a) == DELETED
	//памяь ещё не освобождена?
#define IsGoodPtr(a) Ptr(a) != DELETED
	//Элемент ещё не удален?
#define IsNotDeleted(a) Ptr(a->left) != DELETED && Ptr(a->right) != DELETED && Ptr(a->partner) !=DELETED && Ptr(a->parent)!=DELETED && Ptr(a->child)!= DELETED
	//Элемент уже удален?
#define IsBadPtrFull(a) Ptr(a->left) != DELETED || Ptr(a->right) != DELETED || Ptr(a->partner) !=DELETED || Ptr(a->parent)!=DELETED || Ptr(a->child)!= DELETED
	//Элемент инициализирован?
#define IsNotNull(a) Ptr(a->left) != NULL && Ptr(a->right) != NULL && Ptr(a->partner) !=NULL && Ptr(a->parent)!=NULL && Ptr(a->child)!= NULL

	//Функция сравнения целочисленных ключей
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