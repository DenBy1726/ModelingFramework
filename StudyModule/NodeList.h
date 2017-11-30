#pragma once
#include "Node.h"
#include <vector>

namespace Model
{
	class Node;
	class NodeList
	{
		friend class Node;
	private:
		std::vector<Node*> _list;
		void _AssignIndex();
	public:
	
		void Add(Node* node,int index);
		void Add(Node* node);
		void Remove(int index);
		void Remove();
		int Length() const;

		Node* operator[](int index);

		Node* Get(int index);

		~NodeList();
		
	};
}