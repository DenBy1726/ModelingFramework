/*
18.02.2017 14:37
Конструктор моделей предназначен для конструирования модели из определенного описания и сохранения
их в поток. Корректная реализация предполагает, что модель, сохраненная при помощи метода Save может
быть в точности восстановлена методом Load 
(BETA)
- реализована только загрузка
04.04.2017 19:48
Убраны лишние зависимости
Унифицирован процесс распознания распределений
*/
#pragma once
#include "IDistribution.h"
#include "Model.h"
#include "Node.h"
#include <string>
#include <map>
namespace Model
{
	//Базовая версия сериализации, которя может быть расширена пользователем
	//Класс позвоялет сохранять и загружать модели ТОЛЬКО с экспоненциальным и нормальным распределением
	//также не зависимо от очереди,используемой в модели, будет использоваться 2-3Heap
	class ModelBuilder
	{
		
	public:
		//словарь для отображеиня строкового названия распределения в тип
		std::map<std::string, std::string> Command;
		ModelBuilder();
		//создаёт модель на основе файлового описания
		virtual Model* Load(const std::string&,Queue::QueueType QT = Queue::Heap23);
		//сохраняет модель в строку для возможности в будущем востановить при помощи класс ModelBuilder
		virtual std::string Save(const Model& data);

	protected:
		//Создает дескриптор по текстовому описанию
		virtual Descriptors::DistributionDescriptor CreateGenerator(const std::string&);
		//создает текстовое описание по дескриптору
		virtual std::string SaveGenerator(const Descriptors::DistributionDescriptor&);

	};
}