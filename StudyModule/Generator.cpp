#include "Generator.h"
#include "DistributionBuilder.h"
namespace Model
{

	Generator::Generator(const Descriptors::DistributionDescriptor& dis)
	{
		this->Random = Random::Distribution::Create(dis.Type, dis.ArgList);
		time = 0;
	}

	Generator::Generator(Random::IDistribution<double>* rnd)
	{
		this->Random = rnd;
		time = 0;
	}

	Transact* Generator::CreateTransact()
	{
		//7.1)создать транзакт
		Transact* t = new Transact();
		//7.2)сгенерировать время в которое заявка появится
		int elapsed = Random->Next();
		time += elapsed;
		t->OnStart(time);
		return t;
	}
}