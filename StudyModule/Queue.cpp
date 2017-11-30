/*
21.07.2017 23:29
Для подключения всех очередей ,а также возможности быстрого конструирования очереди.
*/
#pragma once
#include "FIFOQueue.cpp"
#include "PriorityQueue.cpp"
#include "BinaryQueue.cpp"
#include "FibonacciQueue.cpp"
#include "2-3Queue.cpp"

namespace Queue
{
	//создает заданную идентификатором очередь. При этом вызывается конструктор по умолчанию
	template<class T>
	IQueue<T>* Create(QueueType QT)
	{
		IQueue<T>* object;
		switch (QT)
		{

		case QueueType::FIFO:
			object = new FIFOQueue<T>();
			break;

		case QueueType::Standart:
			object = new PriorityQueue<T>();
			break;

		case QueueType::Binary:
			object = new BinaryQueue<T>();
			break;

		case QueueType::Fibonacci:
			object = new FibonacciQueue<T>();
			break;

		case QueueType::Heap23:
			object = new Queue23<T>();
			break;

		default:
			object = nullptr;
			break;
		}
		return object;
	}
}