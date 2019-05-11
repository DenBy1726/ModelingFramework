/*27:01:2017 22:19
—одержит интерфейсную часть сети. —еть это абстракци€, котора€ объедени€ет каналы и матрицу 
переходов.—лужит дл€ предоставлени€ данных сети(каналов и матрицы) а также дл€ расчета следующего
канала, куда перейдет транзакт.“аким образом канал с номером 0 понимаетс€ как генератор за€вок
 аналы с номерами 1..n-1 это обрабатывающие узлы, а канал с номером n это терминатор.
////////////////////
18.02.2017 15:11
¬место генератора пользователь должен отдавать системе описание генератора, что безопаснее
///////////////////
11.03.2017 11:35
—еть может вернуть свой описатель,что позволит сохранить и загрузить сеть
*/
#pragma once
#include "matrix.h"
#include "Node.h"
#include "IRandom.h"
#include "Queue.cpp"
#include "Descriptors.cpp"
#include "DistributionBuilder.h"

#include <vector>

namespace Model
{
	class Node;
	class Network
	{
	private:
		Random::IRandom* generator;
	public:
		//матрица переодов
		BasicMatrix<double> TransitionMatrix;
		//список каналов сети
		std::vector<Node*> Channel;

		~Network();
		
		//handler - дескриптор сети
		Network(const BasicMatrix<double>& transition, const std::vector<Node*> nodes);
		//получить номер канала, в который перейдет транзакт из current
		int GetNextChannel(const int& current);
		//очистить сеть
		void Clear();
		//проверить €вл€етс€ ли current узел терминатором
		bool IsTerminator(const int& current);

		Descriptors::NetworkDescriptor GetDescriptor();
	};

	
}