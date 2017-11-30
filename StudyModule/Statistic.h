#include "StateStatisticUnit.cpp";
#include "WorkStatisticUnit.cpp";
#include "FullStatistic.cpp"
#include "Transact.h"
#include <vector>
namespace Statistic
{
	class StatisticTable
	{
		void OnEnter(::Model::Node* numberOf, const int& time);
		std::vector<int> _channel;
	public:
		std::vector<std::vector<StateStatisticUnit> > StateTable;
		std::vector<std::vector<WorkStatisticUnit> > WorkTable;
		void Register(::Model::Node* node,::Model::Transact* haveEntered, const int& time);
		void OnStart(::Model::Node* node, ::Model::Transact* tr, const int& time);
		void OnRelease(::Model::Node* node, ::Model::Transact* tr, const int& time);
		void UpdateState(::Model::Node* node, const int& time);

		FullStatistic ComputeStatistic(::Model::Terminal* terminal);
		FullStatistic ComputeStatistic(int ammount);

		StatisticTable(int ammountOF);
	};
}