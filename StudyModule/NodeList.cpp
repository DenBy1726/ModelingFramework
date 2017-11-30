#pragma once
#include "NodeList.h"
namespace Model
{
	void NodeList::_AssignIndex()
	{
		for (int i = 0; i < _list.size(); i++)
		{
			_list[i]->id = i;
		}
	}

	void NodeList::Add(Node* node, int index)
	{
		_list.insert(_list.begin() + index, node);
		_AssignIndex();
	}
	void NodeList::Add(Node* node)
	{
		_list.push_back(node);
		_AssignIndex();
	}
	void NodeList::Remove(int index)
	{
		_list.erase(_list.begin() + index);
		_AssignIndex();
	}

	void NodeList::Remove()
	{
		_list.pop_back();
		_AssignIndex();
	}

	Node* NodeList::operator[](int index)
	{
		return _list[index];
	}

	Node* NodeList::Get(int index)
	{
		return _list[index];
	}

	int NodeList::Length() const
	{
		return _list.size();
	}

	NodeList::~NodeList()
	{

	}
}