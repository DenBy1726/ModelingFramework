/*
18.02.2017 14:26
���������� ������������ �������
*/
#pragma once
#include "ModelBuilder.h"
#include "Function.h"

namespace Model
{
	ModelBuilder::ModelBuilder()
	{
		Command["EXP"] = "IntExponentialDistribution";
		Command["UNIFORM"] = "UniformDistribution";
	}

	//������ ������ �� ������ ��������� ��������
	Model* ModelBuilder::Load(const std::string& text, Queue::QueueType QT)
	{

		//�������� ������ �������� ������
		auto tokens = Function::Split(text, '\n');

		//� ������ ������ �������� ���������� �����
		int node_count = Function::FromString<int>(tokens[0]);
		if (node_count < 1)
			throw("���� ������ �������� �� ������� ���� �� ������ ����");

		//��������� �������:
		// ����
		// ����
		// ...
		// �����
		BasicMatrix<double> matrix(node_count + 2);

		for (int i = 0; i < node_count + 2; i++)
		{
			auto matrix_value = Function::GetTokens(tokens[i + 1]);
			if (matrix_value.size() != node_count + 2)
				throw ("�������� ����� ����� ��� �������");

			//�������� ������ �������, � ����� ������� �������� �����
			for (int j = 0; j < node_count + 2; j++)
			{
				matrix[i][j] = Function::FromString<double>(matrix_value[j]);
				if (matrix[i][j] == 0.0)
				{
					std::replace(matrix_value[j].begin(), matrix_value[j].end(), '.', ',');
					matrix[i][j] = Function::FromString<double>(matrix_value[j]);

				}
				//��������� �������
			}
		}

		std::vector<Node*> nodes;

		//��������� ��������� �����
		for (int i = 0; i < node_count; i++)
		{
			auto parts = Function::Split(tokens[node_count + 3 + i],'-');
			int channel;
			Descriptors::DistributionDescriptor dd;
			//������ ������
			if (parts.size() == 1)
			{
				channel = 1;
				dd = CreateGenerator(parts[0]);
				nodes.push_back(new Node(Random::Distribution::Create(dd), channel));
			}
			else
			{
				int channel = Function::FromString<int>(parts[0]);
				dd = CreateGenerator(parts[1]);
				nodes.push_back(new Node(Random::Distribution::Create(dd), channel));
			}
		}

		//��������� ��������� ������
		Descriptors::DistributionDescriptor dd = CreateGenerator(tokens[node_count * 2 + 3]);
		Random::IDistribution<double>* model_generator = Random::Distribution::Create(dd);



		return new Model(matrix, nodes, model_generator, QT);


	}

	std::string ModelBuilder::Save(const Model& model)
	{
		std::string modelSerial;
		Descriptors::ModelDescriptor handler = model.GetDescriptor();
		modelSerial += Function::ToString(handler.Network.NodeDescriptors.size());

		modelSerial += "\n";
		for (int i = 0; i < handler.Network.TransitionMatrix.width(); i++)
		{
			for (int j = 0; j < handler.Network.TransitionMatrix.width(); j++)
			{
				modelSerial += Function::ToString(handler.Network.TransitionMatrix[i][j]);
				modelSerial += " ";
			}
			modelSerial += "\n";
		}
		for each (auto node in handler.Network.NodeDescriptors)
		{
			modelSerial += node.Channel;
			modelSerial += " - ";
			modelSerial += this->SaveGenerator(node.Descriptor);
			modelSerial += "\n";
		}
		modelSerial += this->SaveGenerator(handler.ModelDistribution);
		return modelSerial;
	}


	Descriptors::DistributionDescriptor ModelBuilder::CreateGenerator(const std::string& text)
	{
		//��������� ������ �� ������: ��� �������������, ���������
		auto tokens = Function::Split(text, ':');
		
		if (tokens.size() < 2)
			throw("�������� ������ �������������");

		//��������� ���������
		//�������� ������������� � �������� ��������
		std::transform(tokens[0].begin(), tokens[0].end(), tokens[0].begin(), toupper);
		//������� ��� �������
		tokens[0].erase(std::remove_if(tokens[0].begin(), tokens[0].end(), isspace), tokens[0].end());
		//�������� ������� �� �����
		//std::replace(tokens[1].begin(), tokens[1].end(), '.', ',');
		if (Command.count(tokens[0]) == 0)
			throw("�������� ������������� ������������� " + tokens[0]);
		auto find_item = Command.find(tokens[0]);

		//������� ���������
		auto arg = Function::GetTokens(tokens[1]);


		//��������� ����
		return Descriptors::DistributionDescriptor(find_item->second, arg);

	}

	std::string ModelBuilder::SaveGenerator(const Descriptors::DistributionDescriptor& handler)
	{
		std::string genData;
		std::string dataText = "";
		for each(auto it in Command)
		{
			if (it.second == handler.Type)
				dataText = it.first;
		}

		if (dataText == "")
		{
			throw("����������� ��� �������������");
		}

		genData += dataText;
		genData += " :";
		for (int i = 0; i < handler.ArgList.size(); i++)
		{
			genData += " ";
			genData += std::to_string(handler.ArgList[i]);
		}


		return genData;
	}


}