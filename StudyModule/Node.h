/*27:01:2017 22:25
Содержит интерфейсную часть узла.Узел содержит генератор, при помощи которого определяется
время до окончания окончания обработки транзакта, очередь транзактов к узлу, флаг состояния
канала, а также статистические данные.
////////////////////
09.02.2017 21:10
Изменен интерфейс:
метод BeginCapture(Transact,const int&) для начала захвата канала
метод TryCapture теперь отвечает только за диспетчеризацию дальгейших действий с транзактом
(стать в очередь или занять канал)
метод Capture отвечает за занятие канала.
Удалено поле isWorked. Теперь каждый транзакт сам хранит номер ячейки, с которой связан и в связи
с этим метод Release теперь в качестве аргумента должен принимать указатель на освобожденный транзакт.
Рабочая статистика теперь хранится не в Хэш таблице(оказалось не оправдано), а в массиве.
////////////////////
18.02.2017 15:11
Вместо генератора пользователь должен отдавать системе описание генератора, что безопаснее
07.04.2017 20:35
Добавлен метод регистрации транзакта в канале, что позволяет разбить процесс захвата на части
Добавлен метод проверки состояния канала
Добавлен ряд перегрузок дял удобства работы
/////////////////////
11.04.2017 21:05
Переход на многоканальные устройства. 
Метод IsWorking() заменен на IsBusy()
bool isWorking заменен на int capacity.Означает количество незанятых каналов. Если 0 то устройство занято
Метод Capasity() возвращает текущую свободную емкость канала
Теперь канал задается при помощи NodeDescriptor. Ранее DistributionDescriptor
*/
#pragma once
#include "Time.h"
#include "IDistribution.h"
#include "Transact.h"
#include "IQueue.h"
#include "Event.h"
#include "DistributionBuilder.h"
#include "NodeQueue.h"
#include "NodeList.h"
#include <vector>
namespace Model
{
	class Transact;
	struct Event;
	class NodeQueue;
	class NodeList;
	class Node
	{
		friend class Model;
		friend class NodeList;
		

	private:

		int channel;
		int capacity;	
		int id;
	
	public:
		
		
		NodeQueue* Queue;
		Random::IDistribution<double>* Distribution;

		Node(int capacity);

		Node(Random::IDistribution<double>* dist, int capacity = 1);

		~Node();

		//Попытаться занять канал если он свободен,иначе стать в очередь. 
	//	Event* TryCapture(Transact* haveEntered, const int& time);
		//захват канала
		Event* Capture(Transact* haveEntered, const int& time);

		//Освобождает канал и если в канал была загружена новая заявка, возвращает событие
		//освобождения канала.Так канал не знает текущего времени, time подается текущее время.
		void Release();
		//очистка канала
		void Clear();

		//Работает ли канал
		bool IsBusy();

		//свободная емкость
		int Capacity();

		int Channel();

		int GetId();



	};
}