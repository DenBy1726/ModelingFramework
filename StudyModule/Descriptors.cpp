/*
Здесь описаны дескрипторы, которые используются в модели
Дескриптор представляет собой описание объекта модели, содержащее только данные о конфигурации
но не нарушает инкапсуляции объекта. Дескриптор содержит копии данных, по которым можно восстановить объект
04.04.2017 19:47
Добавлена возможность записи в дескриптор аргументов как массив строк
11.04.2017 21:17
Устройство теперь задается NodeDescriptor вместо DistributionDescriptor
*/
#pragma once
#include "IDistribution.h"
#include "IQueue.h"
#include "matrix.h"
#include "Function.h"
namespace Descriptors
{
	struct NetworkDescriptor;
	struct ModelDescriptor;
	struct NodeDescriptor;
	//Описание распределения хранит необходимый данные для конструирования распределения
	struct DistributionDescriptor
	{
		std::string Type;
		std::vector<double> ArgList;

		DistributionDescriptor()
		{
			Type = "Unknown";
		}

		DistributionDescriptor(const std::string& t, const std::vector<double>& a)
		{
			Type = t;
			ArgList = a;
		}

		DistributionDescriptor(const std::string& t, std::vector<std::string> a)
		{
			std::vector<double> arg;
			for (int i = 0; i < a.size(); i++)
			{
				double val = Function::FromString<double>(a[i]);
				if (val == 0.0)
				{
					std::replace(a[i].begin(), a[i].end(), '.', ',');
					val = Function::FromString<double>(a[i]);
				}
				arg.push_back(val);
			}
			Type = t;
			ArgList = arg;
		}

		DistributionDescriptor(const DistributionDescriptor& dd)
		{
			Type = dd.Type;
			ArgList = dd.ArgList;
		}
	};

	struct NetworkDescriptor
	{
		BasicMatrix<double> TransitionMatrix;
		std::vector<NodeDescriptor> NodeDescriptors;

		NetworkDescriptor()
		{

		}

		NetworkDescriptor(const NetworkDescriptor& net)
		{
			TransitionMatrix = net.TransitionMatrix;
			NodeDescriptors = net.NodeDescriptors;
		}

		NetworkDescriptor(const BasicMatrix<double>& mat, const std::vector<NodeDescriptor>& nodes)
		{
			TransitionMatrix = mat;
			NodeDescriptors = nodes;
		}

	};

	//описание модели позволяет восстановить модель при помощи класса ModelFactory
	struct ModelDescriptor
	{
		NetworkDescriptor Network;
		DistributionDescriptor ModelDistribution;
		Queue::QueueType QueueType;

		ModelDescriptor()
		{

		}

		ModelDescriptor(const ModelDescriptor& dd)
		{
			Network = dd.Network;
			ModelDistribution = dd.ModelDistribution;
			QueueType = dd.QueueType;
		}

		ModelDescriptor(const NetworkDescriptor& net,
			const DistributionDescriptor& model, const Queue::QueueType& QT)
		{
			Network = net;
			ModelDistribution = model;
			QueueType = QT;
		}
	};

	//описания устройства
	struct NodeDescriptor
	{
		DistributionDescriptor Descriptor;
		int Channel;
		Queue::QueueType QT;

		NodeDescriptor()
		{
			Descriptor.Type = "Unknown";
			Channel = 0;
			QT = Queue::FIFO;
		}

		NodeDescriptor(const DistributionDescriptor& t, const int& a, Queue::QueueType = Queue::FIFO)
		{
			Descriptor = t;
			Channel = a;
		}

	
		NodeDescriptor(const NodeDescriptor& dd)
		{
			Descriptor = dd.Descriptor;
			Channel = dd.Channel;
			QT = dd.QT;
		}
	};


	
}