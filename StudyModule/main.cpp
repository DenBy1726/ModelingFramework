#include "ModelBuilder.h"
#include "DistributionBuilder.h"
#include "IDistribution.h"
#include "Node.h"
#include "Model.h"
#include "StreamReport.cpp"
#include "RussianLocale.cpp"
#include <iostream>



void GoTest(std::vector<int>);
void ToFileFromFile();
void SerializationWork();
void DeserializationWork();
void MyDistributionUse();
void HandJob();
void HandJob2();
void HandKarkas();

void main()
{
	//HandKarkas();
	//локаль для вывода русского текста в файл
	std::locale::global(std::locale("Rus"));
	setlocale(0, "");
	std::cout << "Введите путь к файлу модели: ";
	std::string path;
	std::cin >> path;
	std::cout << "Ввелите количество заявок: ";
	int ammount;
	std::cin >> ammount;

	std::ifstream file(path);
	std::string modelText;
	char buff;

	for (; file;)
	{
		file.get(buff);
		if (file.eof()) break;
		modelText += buff;
	}

	file.close();

	Model::ModelBuilder builder;
	Model::Model m = *builder.Load(modelText);

	m.Start(ammount);
	auto Stat = m.ComputeStatistic();

	Report::Locale* loc = new Report::RussianLocale();
	Report::StreamReport reporter(&std::wcout, loc);
	reporter.Name = L"Report ";
	reporter.Print(Stat);

	std::wofstream* savefile = new std::wofstream("output.txt");
	Report::StreamReport reporter2(savefile, loc);
	reporter2.Print(Stat);

	savefile->close();
	std::cout <<"\nКопия отчета сохранена в файл output.txt\n\n";

	delete loc;

	system("pause");













//	HandKarkas();
	//HandJob2();
//	MyDistributionUse();
//	DeserializationWork();
	//GoTest({ 8 });
	//ToFileFromFile();
	/*setlocale(LC_ALL, "Russian");
	std::wcout.imbue(std::locale(".1251"));
	
	Model::ModelBuilder mb;
	BasicMatrix<double> tr(3);
	tr[0][1] = 1;
	tr[1][2] = 1;
	std::vector<Descriptors::DistributionDescriptor> ngen;
	ngen.push_back(Descriptors::DistributionDescriptor(Random::DistributionType::IntExponential, { 1.0/10.0 }));
	Descriptors::DistributionDescriptor modelgen = Descriptors::DistributionDescriptor(Random::DistributionType::IntExponential, {1.0/20.0 });

	Model::Model m(tr,ngen,modelgen,Queue::Heap23);
	m.Start(50000);
	auto s = m.ComputeStatistic();

	//std::ofstream file2("model2.txt");
//	file2 << mb.Save(m);

	Report::Locale* loc = new Report::RussianLocale();
	//std::wofstream* file = new std::wofstream("input.txt");
	Report::StreamReport reporter(&std::wcout,loc);
	reporter.Print(s);
//	file->close();*/
}

void ToFileFromFile()
{
	std::ifstream file("model2.txt");
	std::string modelText;
	char buff;

	for (; file;)
	{
		file.get(buff);
		if (file.eof()) break;
		modelText += buff;
	}

	file.close();

	Model::ModelBuilder builder;
	Model::Model m = *builder.Load(modelText);
	m.Start(1000);
	auto Stat = m.ComputeStatistic();
	modelText = builder.Save(m);
	
	std::ofstream file2("model3.txt");
	file2 << modelText;
	file2.close();
}

/*
AMOUNT

0 1 0
0 0 1
0 0 0

TYPE : PARAMS ;
============================================
TYPE:
EXP : lambda ;
UNIFORM : min max ;
============================================
Отчет:
---------------------
Количество транзактов:
Время:
Модельное время:
Количество устройств:
--------------------
для каждого:
--------------------
Номер:
Загрузка канала (ro) :
Средняя длинна очереди (l) :
Среднее число транзактов канале (m) :
Среднее время пребывания в очереди (w) :
Среднее время работы (u) :
-------------------
Системные характеристики:
Средняя длинна очереди (l) :
Среднее число транзактов канале (m) :
Среднее время пребывания в очереди (w) :
Среднее время работы (u) :


*/