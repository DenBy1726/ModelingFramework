#pragma once
#include "Generator.h"
namespace Model
{


	Generator::Generator()
	{
		time = 0;
	}

	Generator::Generator(Random::IDistribution<double>* rnd)
	{
		this->Distribution = rnd;
		time = 0;
	}

	Generator::~Generator()
	{
	}
	Transact* Generator::CreateTransact()
	{
		//7.1)������� ��������
		Transact* t = new Transact();
		//7.2)������������� ����� � ������� ������ ��������
		int elapsed = Distribution->Next();
		time += elapsed;
		t->OnStart(time);
		return t;
	}

	void Generator::Clear()
	{
		time = 0;
	}
}
