#include "Network.h"

namespace Model
{
	Network::Network(const BasicMatrix<double>& transition, const std::vector<Node*> nodes)
	{
		this->TransitionMatrix = transition;
		if (this->TransitionMatrix.width() != this->TransitionMatrix.height())
			throw ("ћатрица переходов должна быть квадратной");
		//отнимаем 2 так как нулевой это генератор, а последний это терминатор. ќни задаютс€ не€вно.
		if (nodes.size() != this->TransitionMatrix.width() - 2)
			throw (" оличество генераторов должно равн€тс€ количеству узлов");
		this->Channel = nodes;
		this->generator = new Random::Random();
	}


	Network::~Network()
	{
		Clear();
		delete this->generator;
		for each(Node* node in this->Channel)
		{
			delete node;
		}
	}


	//получить номер канала, в который перейдет транзакт из current
	/*
		иде€ алгоритма в том что сначала генерируетс€ число 0 <= p <= 1
		далее заводитс€ счетчик
		идем по веро€тност€м на текущем узле, суммируем в счетчик и если
		сумма больше p то возвращаем i -ый узел.
		ѕример:
		ћатрица : 0 0.5 0.5 0 0
		0 0   0   0 1
		0 0   0   1 0
		0 0.7 0   0 0.3
		0 0   0   0 0
		“екущий узел: 0
		p: 0.78

		невозможно перейти в источник, поэтому остчет с первого узла
		1:   0 + 0.5 = 0.5 < 0.78
		2:   0.5 + 0.5 = 1 > 0.78 переходим в 2
		*/
	int Network::GetNextChannel(const int& current)
	{
		//число дл€ определени€ нового узла
		double probValue = this->generator->NextDouble();
		int length = this->TransitionMatrix.height();
		double probSum = 0;
		//с одного так как нельз€ перейти на источник
		for (int i = 1; i < length; i++)
		{
			probSum += this->TransitionMatrix[current][i];
			if (probSum > probValue)
				return i;
		}
		throw("—умма веро€тностей переходов в строке матрицы не равно 1");
	}

	//очистить сеть
	void Network::Clear()
	{
		for each(Node* node in this->Channel)
		{
			node->Clear();
		}
		this->Channel.clear();
		this->TransitionMatrix = BasicMatrix<double>();
	}

	//проверить €вл€етс€ ли current узел терминатором
	//терминатор если номер узла n ,где n размерность матрицы
	bool Network::IsTerminator(const int& current)
	{
		if (current == this->TransitionMatrix.height()-1)
			return true;
		else
			return false;
	}

	Descriptors::NetworkDescriptor Network::GetDescriptor()
	{
		std::vector<Descriptors::NodeDescriptor> nodeDescriptors;
		for each(auto it in this->Channel)
		{
			nodeDescriptors.push_back(it->GetDescriptor());
		}
		return Descriptors::NetworkDescriptor(this->TransitionMatrix, nodeDescriptors);
	}

}