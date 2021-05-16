#include "Headers.h"

HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE); // Для установки цветов

constexpr auto   INF = INT16_MAX;  // Числовой аналог бесконечности.
int**		     MapCities;        // Матрица с расстояниями городов, введённых пользователем.
int              FullWay = 0;	   // Длина каждого расчитанного пути.
bool*            VisitedCities;    // Матрица с метками посещённых городов.
int              AmountCities = 0; // Количество городов.
std::vector<int> IndexesCities;    // Индексы городов в порядке их посещения.

/*
* Принцип работы алгоритма:
* В каждой строке сформированной матрицы, начиная с нулевой, находим кратчайшее расстояние.
* Если город с таким индексом ещё не был посещён, едем в него, отметив, что мы его посетили.
* Вместе с этим прибавили кратчайшее расстояние в строке к общей длине рассчитываемого пути
* Пока не закончатся все города для посещения, мы ездим по городам и прибавляем расстояния.
* В конце прибавляем расстояние от оставшегося города до города, с которого вы начали путешествие, так нужно в него вернуться (по логике задачи коммивояжера).
* Получаем несколько возможных вариантов, из которого выбираем наименьшую длину маршрута.
*
* Небольшой пример работы улучшения:
* Программе был дан следующий список: Берлин,Лейпциг,Ганновер,Гамбург,Эссен,Дюссельдорф,Дрезден
* Если действовать строго по жадному алгоритму, длина маршрута составит 1773 км (Берлин -> Лейпциг -> Дрезден -> Ганновер -> Гамбург -> Эссен -> Дюссельдорф -> Берлин).
* Однако программа нашла путь длиной 1508 км (Берлин -> Гамбург -> Ганновер -> Эссен -> Дюссельдорф -> Лейпциг -> Дрезден -> Берлин).
*/

int FindShortestWay(int Index) // Основной рекурсивный алгоритм нахождения пути.
{
	// На вход принимает индекс города, он же является индексом строки,
	// в которой ведётся поиск наименьшего кратчайшего расстояния.
	IndexesCities.push_back(Index);
	int MinIndex = 0;			 // Следующий индекс города, в который направлен минимальный путь.
	int Min = INF;		         // Для нахождения минимума сначала приравняем к большому числу.
	VisitedCities[Index] = 1;    // Отмечаем, что в городе с данным индексом мы побывали.
	for (int i = 0; i < AmountCities; ++i) // Начинаем цикл по строке с полученным индексом.
		// Если ячейка матрицы в данной строке больше 0, не бесконечна, мы ещё не бывали в этом городе, 
		// и расстояние до него меньше ранее рассчитанного минимального.
		if (MapCities[Index][i] != INF && MapCities[Index][i] < Min && VisitedCities[i] != 1 && MapCities[Index][i] > 0)
		{
			// То он может рассматриваться, как город, до которого путь наименьший.
			Min = MapCities[Index][i]; // Запомнили минимальное расстояние.
			MinIndex = i;              // Запомнили индекс города, до которого в строке наименьшее расстояние.
		}
	// К этому моменту получили минимальное расстояние и индекс нужного города.
	// Пока остаётся хоть один город, который мы не посетили, переменная Min не будет равна бесконечности.
	if (Min != INF)
	{
		FullWay += Min;			   // Прибавляем полученное минимальное расстояние.
		FindShortestWay(MinIndex); // И снова вызываем функцию, передав ей индекс города, расстояние до которого здесь было минимальным.
	}
	else
	{
		// Блок выполниться, если все города мы уже прошли.
		return Index; // Возвращаем индекс последнего посещённого города.
		// Здесь рекурсия заканчивается.
	}
}

void Continue() // Функция выступает заглушкой в конце выполнения каждого из пунктов меню.
{
	SetConsoleTextAttribute(hConsole, (WORD)Green | (WORD)Black);
	printf("\nДля продолжения нажмите любую клавишу...\n");
	SetConsoleTextAttribute(hConsole, (WORD)LightBlue | (WORD)Black);
	char* Continue = new char;
	*Continue = getchar();
	delete Continue;
}

void getDirections() // Построить маршрут
{
	SetConsoleTextAttribute(hConsole, (WORD)LightBlue | (WORD)Black);
	
	system("cls");
	FILE** TRAVELS = new FILE*;
	if (!fopen_s(TRAVELS, TRAVELS_FILE_NAME, "rb"))
	{
		SetConsoleTextAttribute(hConsole, (WORD)Yellow | (WORD)Black);
		printf("-------------------------------------------------------------\n");
		printf("---------------------Построение-маршрута---------------------\n");
		printf("-------------------------------------------------------------\n\n");
		SetConsoleTextAttribute(hConsole, (WORD)LightBlue | (WORD)Black);

		// Формируем список доступных городов. 
		// Читаем из файла часть записи "Откуда" и, если её нет в векторе, заталкиваем туда.
		std::vector<std::string> AvailableCities;
		Trip* FromFile = new Trip;
		if (*TRAVELS)
		{
			while (!feof(*TRAVELS))
			{
				fread(FromFile->from, sizeof(char), LENGTH_CITY_NAME, *TRAVELS);
				fread(FromFile->to, sizeof(char), LENGTH_CITY_NAME, *TRAVELS);
				fread(&FromFile->distance, sizeof(int), 1, *TRAVELS);

				// Находим, есть ли такая же запись в векторе.
				// За контейнер можно было взять и множество, но по нему нельзя произвольно индексироваться.
				auto result = std::find(begin(AvailableCities), end(AvailableCities), FromFile->from);
				if (strcmp(FromFile->from, " ") && result == end(AvailableCities))
					AvailableCities.push_back(FromFile->from);
			}
		}

		printf("\nДля построения маршрута введите номера городов из следующего списка.\n\n");
		for (int i = 0; i < AvailableCities.size(); ++i)
			printf("%i. %s\n", i, AvailableCities[i].c_str());

		printf("\nНомера городов через пробел: ");
		char* CitiesToVisit = new char[LENGTH_CITY_NAME * 50];
		gets_s(CitiesToVisit, LENGTH_CITY_NAME * 50);

		std::vector<std::string> charCities;
		std::stringstream ss(CitiesToVisit);
		std::string City;
		while (ss.good())
		{
			getline(ss, City, ' ');
			charCities.push_back(City);
		}

		std::vector<int> intCities; // сюда положим индексы городов
		for (auto i : charCities)
			intCities.push_back(atoi(i.c_str()));

		std::sort(begin(intCities), end(intCities), [](int One, int Two)
			{
				return One < Two;
			});

		std::vector<std::string> Cities; // здесь будут имена городов в символьном виде

		for (int i = 0; i < charCities.size(); ++i)
			Cities.push_back(AvailableCities[intCities[i]]);

		delete[] CitiesToVisit;

		AmountCities = Cities.size();
		MapCities = new int* [AmountCities];      // Будущая матрица расстояний.
		for (int i = 0; i < AmountCities; ++i)
			MapCities[i] = new int[AmountCities];
		VisitedCities = new bool[AmountCities];   // Отметки о посещённых городах.

		// Формируем матрицу расстояний, сверяя имена городов в списке пользователя и в считанных записях из файла.
		fseek(*TRAVELS, SEEK_SET, 0);
		if (*TRAVELS)
		{
			for(int i = 0; i<AmountCities;++i)
				for (int j = 0; j < AmountCities; ++j)
				{
					if (i == j) 
					{ 
						MapCities[i][j] = 0; // По диагонали все элементы нулевые (недостижимые).
						continue;
					}
					while (!feof(*TRAVELS))
					{
						fread(FromFile->from, sizeof(char), LENGTH_CITY_NAME, *TRAVELS);
						fread(FromFile->to, sizeof(char), LENGTH_CITY_NAME, *TRAVELS);
						fread(&FromFile->distance, sizeof(int), 1, *TRAVELS);

						if (!strcmp(Cities[i].c_str(), FromFile->from) &&
							!strcmp(Cities[j].c_str(), FromFile->to))
						{
							MapCities[i][j] = FromFile->distance; // Вносим расстояние в матрицу.
							break;
						}
					}
				}
		}
		delete FromFile;

		printf("\nПуть: ");	// Рядом будут выведены имена городов, образующих путь.

		/*
		* Алгоритм ближайшего соседа жадный, но с небольшим улучшением.
		* Ниже перебираются все "жадные" пути из всех городов (начальная точка каждый раз меняется).
		* Затем выбирается наилучший путь.
		*/

		std::vector<int> AllLengths;               // Длины всех маршрутов, которые нашёл алгоритм, каждый раз меняя точку отправления.
		for (int i = 0; i < AmountCities; ++i)     // Стартует цикл, перебирающий все возможные точки отправления.
		{
			int lastIndex = FindShortestWay(i);    // Передаём каждую точку в алгоритм, и принимаем индекс последнего посещенного города.
			FullWay += MapCities[lastIndex][i];    // Прибавляем путь на возрат (маршрут кольцевой).
			IndexesCities.push_back(i);            // Вносим в вектор индекс города, к которому надо вернуться. (В самом векторе все маршруты идут подряд).
			AllLengths.push_back(FullWay);         // Запись о длине рассчитанного пути вносим в вектор.
			FullWay = 0;                           // Обнуляем путь.
			for (int j = 0; j < AmountCities; ++j) // Обнуляем метки посещенных городов, поскольку пойдём их ещё раз.
				VisitedCities[j] = 0;
		}
		
		int minLength = INF; // Найдём наименьшую длину.
		int needIndex = 0;   // Сюда впишем индекс маршрута, с которого начинаются нужные нам города в векторе IndexesCities (маршрут здесь - несколько городов).
		for (int i = 0; i < AmountCities; ++i)
		{
			if (AllLengths[i] < minLength)
			{
				minLength = AllLengths[i];
				needIndex = i;             // Здесь находим индекс записи, с которого начинаются нужные нам города.
			}
		}

		SetConsoleTextAttribute(hConsole, (WORD)Green | (WORD)Black);

		// Выводим маршрут и его длину
		for (int i = 0; i < AmountCities; ++i)
		{
			if (i == needIndex) // Если мы нашли позицию, с которой начинаются нужные нам индексы городов.
			{
				for (int j = needIndex; j < AmountCities + needIndex; ++j)                         // То читаем их индексы и сопоставляем их с именами, затем выводим на экран.
					printf("%s -> ", Cities[IndexesCities[needIndex * AmountCities + j]].c_str());
				printf("%s", Cities[IndexesCities[needIndex * (AmountCities + 1)]].c_str());       // Выводим город, с которого следует отправление, чтобы закольцевать маршрут.
				printf("\n\nДлина рассчитанного пути: %d\n", AllLengths[needIndex]);                // Выводим длину маршрута
				break;
			}
		}

		SetConsoleTextAttribute(hConsole, (WORD)LightBlue | (WORD)Black);

		// Очишаем всё.
		AllLengths.clear();
		Cities.clear();
		AvailableCities.clear();
		IndexesCities.clear();
		delete [] VisitedCities;
		for (int i = 0; i < AmountCities; ++i)
			delete MapCities[i];
		delete[] MapCities;
	}
	else
	{
		SetConsoleTextAttribute(hConsole, (WORD)Red | (WORD)Black);
		printf("Файл %s не удалось открыть для чтения.\n", TRAVELS_FILE_NAME);
		printf("Проверьте наличие файла, его расположение и наличие прав на запись.\n");
		printf("Затем перезапустите программу.\n");
		SetConsoleTextAttribute(hConsole, (WORD)LightBlue | (WORD)Black);
	}

	fclose(*TRAVELS);
	delete TRAVELS;

	Continue();
}

void showWays() // Показать маршруты
{
	SetConsoleTextAttribute(hConsole, (WORD)LightBlue | (WORD)Black);
	
	system("cls");
	FILE** TRAVELS = new FILE*;
	if (!fopen_s(TRAVELS, TRAVELS_FILE_NAME, "rb"))
	{
		SetConsoleTextAttribute(hConsole, (WORD)Yellow | (WORD)Black);
		printf("----------------------------------------------------------------\n");
		printf("---------------------------Маршруты-----------------------------\n");
		printf("----------------------------------------------------------------\n");
		SetConsoleTextAttribute(hConsole, (WORD)LightBlue | (WORD)Black);

		// Читаем из файла все записи и выводим в консоль
		int numberRecord = 1;
		Trip* ShowTrip = new Trip;
		if (*TRAVELS)
		{
			while (!feof(*TRAVELS))
			{
				ShowTrip->distance = 0;

				fread(ShowTrip->from, sizeof(char), LENGTH_CITY_NAME, *TRAVELS);
				fread(ShowTrip->to, sizeof(char), LENGTH_CITY_NAME, *TRAVELS);
				fread(&ShowTrip->distance, sizeof(int), 1, *TRAVELS);

				if (ShowTrip->distance != 0)
				{
					printf("%-5d|", numberRecord); 
					++numberRecord;
					printf("%-25s|", ShowTrip->from);
					printf("%-25s|", ShowTrip->to);
					printf("%-5d|\n", ShowTrip->distance);
					printf("----------------------------------------------------------------\n");
				}
			}
		}

		delete ShowTrip;
	}
	else
	{
		SetConsoleTextAttribute(hConsole, (WORD)Red | (WORD)Black);
		printf("Файл %s не удалось открыть для чтения.\n", TRAVELS_FILE_NAME);
		printf("Проверьте наличие файла, его расположение и наличие прав на запись.\n");
		printf("Затем перезапустите программу.\n");
		SetConsoleTextAttribute(hConsole, (WORD)LightBlue | (WORD)Black);
	}
	if (*TRAVELS) fclose(*TRAVELS);
	delete TRAVELS;

	Continue();
}

void addWay() // Добавить маршрут.
{
	SetConsoleTextAttribute(hConsole, (WORD)LightBlue | (WORD)Black);
	
	system("cls");
	FILE** TRAVELS = new FILE*;
	if (!fopen_s(TRAVELS, TRAVELS_FILE_NAME, "rb+"))
	{
		SetConsoleTextAttribute(hConsole, (WORD)Yellow | (WORD)Black);
		printf("----------------------------------\n");
		printf("---------Добавление-записи--------\n");
		printf("----------------------------------\n\n");
		SetConsoleTextAttribute(hConsole, (WORD)LightBlue | (WORD)Black);

		std::vector<Trip> AllTrips; // Сюда запишем все поездки из файла.
		printf("Введите данные добавляемой записи:\n\n");
		// Запросим добавляемую запись.
		Trip* AddedTrip = new Trip();
		printf("Пункт отправления: ");
		gets_s(AddedTrip->from, LENGTH_CITY_NAME);
		printf("Пункт Назначения: ");
		gets_s(AddedTrip->to, LENGTH_CITY_NAME);
		printf("Расстояние: ");
		scanf_s("%d", &AddedTrip->distance);
		
		// Добавим запись, которую ввёл пользователь, и обратную ей (предполагается, что пути симметричны).
		AllTrips.push_back(*AddedTrip);
		Trip* ReverseAddedTrip = new Trip(AddedTrip->to, AddedTrip->from, AddedTrip->distance);
		AllTrips.push_back(*ReverseAddedTrip);

		delete AddedTrip;
		delete ReverseAddedTrip;
		
		// Читаем из файла все поездки.
		Trip* TripFromFile = new Trip;
		if (*TRAVELS) 
		{ 
			fseek(*TRAVELS, SEEK_SET, 0); 

			while (!feof(*TRAVELS))
			{
				TripFromFile->distance = 0;

				fread(TripFromFile->from, sizeof(char), LENGTH_CITY_NAME, *TRAVELS);
				fread(TripFromFile->to, sizeof(char), LENGTH_CITY_NAME, *TRAVELS);
				fread(&TripFromFile->distance, sizeof(int), 1, *TRAVELS);

				if (TripFromFile->distance != 0) AllTrips.push_back(*TripFromFile);
			}
		}

		delete TripFromFile;

		// При помощи быстрой сортировки из STL сортируем вектор поездок в лексикографическом порядке возрастания.
		std::sort(begin(AllTrips), end(AllTrips), [](const Trip& Obj1, const Trip& Obj2) 
			{
				if (strcmp(Obj1.from, Obj2.from) == 0)
					return strcmp(Obj1.to, Obj2.to) == -1;
				else
					return strcmp(Obj1.from, Obj2.from) == -1;
			});

		// Открываем теперь уже чистый файл для записи всех поездок вместе с добавленной.
		fclose(*TRAVELS);
		remove(TRAVELS_FILE_NAME);
		if (!fopen_s(TRAVELS, TRAVELS_FILE_NAME, "wb"))
		{
			if (*TRAVELS)
			{
				fseek(*TRAVELS, SEEK_SET, 0);

				for (const Trip& trip : AllTrips)
				{
					fwrite(trip.from, sizeof(char), LENGTH_CITY_NAME, *TRAVELS);
					fwrite(trip.to, sizeof(char), LENGTH_CITY_NAME, *TRAVELS);
					fwrite(&trip.distance, sizeof(int), 1, *TRAVELS);
				}
			}
		}
	}
	else
	{
		SetConsoleTextAttribute(hConsole, (WORD)Red | (WORD)Black);
		printf("Файл %s не удалось открыть для дозаписи.\n", TRAVELS_FILE_NAME);
		printf("Проверьте наличие файла, его расположение и наличие прав на запись.\n");
		printf("Затем перезапустите программу.\n");
		SetConsoleTextAttribute(hConsole, (WORD)LightBlue | (WORD)Black);
	}

	if(*TRAVELS) fclose(*TRAVELS);
	delete TRAVELS;
	
	Continue();
}

void delWay() // Удалить маршрут.
{
	SetConsoleTextAttribute(hConsole, (WORD)LightBlue | (WORD)Black);

	system("cls");
	FILE** TRAVELS = new FILE*;

	if (!fopen_s(TRAVELS, TRAVELS_FILE_NAME, "rb"))
	{
		// Запрашиваем данные записи для удаления.
		SetConsoleTextAttribute(hConsole, (WORD)Yellow | (WORD)Black);
		printf("-------------------------------------------------\n");
		printf("-----------------Удаление-записи-----------------\n");
		printf("-------------------------------------------------\n\n");
		SetConsoleTextAttribute(hConsole, (WORD)LightBlue | (WORD)Black);

		printf("Введите данные записи, которую требуется удалить:\n\n");
		Trip* TripToDelete = new Trip;
		printf("Пункт отправления: ");
		gets_s(TripToDelete->from, LENGTH_CITY_NAME);
		printf("Пункт назначения: ");
		gets_s(TripToDelete->to, LENGTH_CITY_NAME);
		printf("Расстояние: ");
		scanf_s("%d", &TripToDelete->distance);
		// Создаём запись, обратную введённой, её тоже удалим из списка поездок, поскольку ранее добавлялись обе.
		Trip* reverseTripDelete = new Trip(TripToDelete->to, TripToDelete->from, TripToDelete->distance);

		std::vector<Trip>AllTripsWithoutDeleted; // Сюда поместим все записи, не включая удаляемые, их просто пропустим.
		Trip* TripFromFile = new Trip;
		if (*TRAVELS)
		{
			fseek(*TRAVELS, SEEK_SET, 0);

			while (!feof(*TRAVELS))
			{
				TripFromFile->distance = 0;

				fread(TripFromFile->from, sizeof(char), LENGTH_CITY_NAME, *TRAVELS);
				fread(TripFromFile->to, sizeof(char), LENGTH_CITY_NAME, *TRAVELS);
				fread(&TripFromFile->distance, sizeof(int), 1, *TRAVELS);

				// Сравниваем запись из файла с записями, которые введены пользователем для удаления.
				// Если записи не одинаковые, заталкиваем её в вектор.
				if (TripFromFile->distance != 0 && 
					(strcmp(TripFromFile->from, TripToDelete->from) || strcmp(TripFromFile->to, TripToDelete->to) || TripFromFile->distance != TripToDelete->distance) &&
					(strcmp(TripFromFile->from, reverseTripDelete->from) || strcmp(TripFromFile->to, reverseTripDelete->to) || TripFromFile->distance != reverseTripDelete->distance))
						AllTripsWithoutDeleted.push_back(*TripFromFile);
			}
		}

		// Переписываем вновь открытый файл, добавляя все записи, кроме удалённых
		if (*TRAVELS) fclose(*TRAVELS);
		remove(TRAVELS_FILE_NAME);

		if (!fopen_s(TRAVELS, TRAVELS_FILE_NAME, "ab"))
		{
			if (*TRAVELS)
			{
				fseek(*TRAVELS, SEEK_SET, 0);
				for (const Trip& Item : AllTripsWithoutDeleted)
				{
					fwrite(Item.from, sizeof(char), LENGTH_CITY_NAME, *TRAVELS);
					fwrite(Item.to, sizeof(char), LENGTH_CITY_NAME, *TRAVELS);
					fwrite(&Item.distance, sizeof(int), 1, *TRAVELS);
				}
			}
		}
	}
	else
	{
		SetConsoleTextAttribute(hConsole, (WORD)Red | (WORD)Black);
		printf("Файл %s не удалось открыть для изменение.\n", TRAVELS_FILE_NAME);
		printf("Проверьте наличие файла, его расположение и наличие прав на запись.\n");
		printf("Затем перезапустите программу.\n");
		SetConsoleTextAttribute(hConsole, (WORD)LightBlue | (WORD)Black);
	}

	if (*TRAVELS) fclose(*TRAVELS);
	delete TRAVELS;

	Continue();
}
