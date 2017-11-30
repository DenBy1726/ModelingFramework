/*12.04.2017 19:54

*/
#pragma once
#include "IQueue.h"
#include "Transact.h"
#include <vector>
namespace Model
{
	class Transact;
	class NodeQueue
	{
		Queue::IQueue<Transact*>* data;
	public:

		//задается базовая очередь, на которой будет строится очередь событий
		NodeQueue(Queue::QueueType QT = Queue::Heap23);

		~NodeQueue();

		void Clear();

		int Length();

		bool Empty();

		void Add(Transact* e);

		Transact* Remove();

		Queue::QueueType Type();
	};
}