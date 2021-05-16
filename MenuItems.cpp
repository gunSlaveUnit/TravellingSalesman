#include "Headers.h"

HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE); // ��� ��������� ������

constexpr auto   INF = INT16_MAX;  // �������� ������ �������������.
int**		     MapCities;        // ������� � ������������ �������, �������� �������������.
int              FullWay = 0;	   // ����� ������� ������������ ����.
bool*            VisitedCities;    // ������� � ������� ���������� �������.
int              AmountCities = 0; // ���������� �������.
std::vector<int> IndexesCities;    // ������� ������� � ������� �� ���������.

/*
* ������� ������ ���������:
* � ������ ������ �������������� �������, ������� � �������, ������� ���������� ����������.
* ���� ����� � ����� �������� ��� �� ��� �������, ���� � ����, �������, ��� �� ��� ��������.
* ������ � ���� ��������� ���������� ���������� � ������ � ����� ����� ��������������� ����
* ���� �� ���������� ��� ������ ��� ���������, �� ����� �� ������� � ���������� ����������.
* � ����� ���������� ���������� �� ����������� ������ �� ������, � �������� �� ������ �����������, ��� ����� � ���� ��������� (�� ������ ������ ������������).
* �������� ��������� ��������� ���������, �� �������� �������� ���������� ����� ��������.
*
* ��������� ������ ������ ���������:
* ��������� ��� ��� ��������� ������: ������,�������,��������,�������,�����,�����������,�������
* ���� ����������� ������ �� ������� ���������, ����� �������� �������� 1773 �� (������ -> ������� -> ������� -> �������� -> ������� -> ����� -> ����������� -> ������).
* ������ ��������� ����� ���� ������ 1508 �� (������ -> ������� -> �������� -> ����� -> ����������� -> ������� -> ������� -> ������).
*/

int FindShortestWay(int Index) // �������� ����������� �������� ���������� ����.
{
	// �� ���� ��������� ������ ������, �� �� �������� �������� ������,
	// � ������� ������ ����� ����������� ����������� ����������.
	IndexesCities.push_back(Index);
	int MinIndex = 0;			 // ��������� ������ ������, � ������� ��������� ����������� ����.
	int Min = INF;		         // ��� ���������� �������� ������� ���������� � �������� �����.
	VisitedCities[Index] = 1;    // ��������, ��� � ������ � ������ �������� �� ��������.
	for (int i = 0; i < AmountCities; ++i) // �������� ���� �� ������ � ���������� ��������.
		// ���� ������ ������� � ������ ������ ������ 0, �� ����������, �� ��� �� ������ � ���� ������, 
		// � ���������� �� ���� ������ ����� ������������� ������������.
		if (MapCities[Index][i] != INF && MapCities[Index][i] < Min && VisitedCities[i] != 1 && MapCities[Index][i] > 0)
		{
			// �� �� ����� ���������������, ��� �����, �� �������� ���� ����������.
			Min = MapCities[Index][i]; // ��������� ����������� ����������.
			MinIndex = i;              // ��������� ������ ������, �� �������� � ������ ���������� ����������.
		}
	// � ����� ������� �������� ����������� ���������� � ������ ������� ������.
	// ���� ������� ���� ���� �����, ������� �� �� ��������, ���������� Min �� ����� ����� �������������.
	if (Min != INF)
	{
		FullWay += Min;			   // ���������� ���������� ����������� ����������.
		FindShortestWay(MinIndex); // � ����� �������� �������, ������� �� ������ ������, ���������� �� �������� ����� ���� �����������.
	}
	else
	{
		// ���� �����������, ���� ��� ������ �� ��� ������.
		return Index; // ���������� ������ ���������� ����������� ������.
		// ����� �������� �������������.
	}
}

void Continue() // ������� ��������� ��������� � ����� ���������� ������� �� ������� ����.
{
	SetConsoleTextAttribute(hConsole, (WORD)Green | (WORD)Black);
	printf("\n��� ����������� ������� ����� �������...\n");
	SetConsoleTextAttribute(hConsole, (WORD)LightBlue | (WORD)Black);
	char* Continue = new char;
	*Continue = getchar();
	delete Continue;
}

void getDirections() // ��������� �������
{
	SetConsoleTextAttribute(hConsole, (WORD)LightBlue | (WORD)Black);
	
	system("cls");
	FILE** TRAVELS = new FILE*;
	if (!fopen_s(TRAVELS, TRAVELS_FILE_NAME, "rb"))
	{
		SetConsoleTextAttribute(hConsole, (WORD)Yellow | (WORD)Black);
		printf("-------------------------------------------------------------\n");
		printf("---------------------����������-��������---------------------\n");
		printf("-------------------------------------------------------------\n\n");
		SetConsoleTextAttribute(hConsole, (WORD)LightBlue | (WORD)Black);

		// ��������� ������ ��������� �������. 
		// ������ �� ����� ����� ������ "������" �, ���� � ��� � �������, ����������� ����.
		std::vector<std::string> AvailableCities;
		Trip* FromFile = new Trip;
		if (*TRAVELS)
		{
			while (!feof(*TRAVELS))
			{
				fread(FromFile->from, sizeof(char), LENGTH_CITY_NAME, *TRAVELS);
				fread(FromFile->to, sizeof(char), LENGTH_CITY_NAME, *TRAVELS);
				fread(&FromFile->distance, sizeof(int), 1, *TRAVELS);

				// �������, ���� �� ����� �� ������ � �������.
				// �� ��������� ����� ���� ����� � ���������, �� �� ���� ������ ����������� ���������������.
				auto result = std::find(begin(AvailableCities), end(AvailableCities), FromFile->from);
				if (strcmp(FromFile->from, " ") && result == end(AvailableCities))
					AvailableCities.push_back(FromFile->from);
			}
		}

		printf("\n��� ���������� �������� ������� ������ ������� �� ���������� ������.\n\n");
		for (int i = 0; i < AvailableCities.size(); ++i)
			printf("%i. %s\n", i, AvailableCities[i].c_str());

		printf("\n������ ������� ����� ������: ");
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

		std::vector<int> intCities; // ���� ������� ������� �������
		for (auto i : charCities)
			intCities.push_back(atoi(i.c_str()));

		std::sort(begin(intCities), end(intCities), [](int One, int Two)
			{
				return One < Two;
			});

		std::vector<std::string> Cities; // ����� ����� ����� ������� � ���������� ����

		for (int i = 0; i < charCities.size(); ++i)
			Cities.push_back(AvailableCities[intCities[i]]);

		delete[] CitiesToVisit;

		AmountCities = Cities.size();
		MapCities = new int* [AmountCities];      // ������� ������� ����������.
		for (int i = 0; i < AmountCities; ++i)
			MapCities[i] = new int[AmountCities];
		VisitedCities = new bool[AmountCities];   // ������� � ���������� �������.

		// ��������� ������� ����������, ������ ����� ������� � ������ ������������ � � ��������� ������� �� �����.
		fseek(*TRAVELS, SEEK_SET, 0);
		if (*TRAVELS)
		{
			for(int i = 0; i<AmountCities;++i)
				for (int j = 0; j < AmountCities; ++j)
				{
					if (i == j) 
					{ 
						MapCities[i][j] = 0; // �� ��������� ��� �������� ������� (������������).
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
							MapCities[i][j] = FromFile->distance; // ������ ���������� � �������.
							break;
						}
					}
				}
		}
		delete FromFile;

		printf("\n����: ");	// ����� ����� �������� ����� �������, ���������� ����.

		/*
		* �������� ���������� ������ ������, �� � ��������� ����������.
		* ���� ������������ ��� "������" ���� �� ���� ������� (��������� ����� ������ ��� ��������).
		* ����� ���������� ��������� ����.
		*/

		std::vector<int> AllLengths;               // ����� ���� ���������, ������� ����� ��������, ������ ��� ����� ����� �����������.
		for (int i = 0; i < AmountCities; ++i)     // �������� ����, ������������ ��� ��������� ����� �����������.
		{
			int lastIndex = FindShortestWay(i);    // ������� ������ ����� � ��������, � ��������� ������ ���������� ����������� ������.
			FullWay += MapCities[lastIndex][i];    // ���������� ���� �� ������ (������� ���������).
			IndexesCities.push_back(i);            // ������ � ������ ������ ������, � �������� ���� ���������. (� ����� ������� ��� �������� ���� ������).
			AllLengths.push_back(FullWay);         // ������ � ����� ������������� ���� ������ � ������.
			FullWay = 0;                           // �������� ����.
			for (int j = 0; j < AmountCities; ++j) // �������� ����� ���������� �������, ��������� ����� �� ��� ���.
				VisitedCities[j] = 0;
		}
		
		int minLength = INF; // ����� ���������� �����.
		int needIndex = 0;   // ���� ������ ������ ��������, � �������� ���������� ������ ��� ������ � ������� IndexesCities (������� ����� - ��������� �������).
		for (int i = 0; i < AmountCities; ++i)
		{
			if (AllLengths[i] < minLength)
			{
				minLength = AllLengths[i];
				needIndex = i;             // ����� ������� ������ ������, � �������� ���������� ������ ��� ������.
			}
		}

		SetConsoleTextAttribute(hConsole, (WORD)Green | (WORD)Black);

		// ������� ������� � ��� �����
		for (int i = 0; i < AmountCities; ++i)
		{
			if (i == needIndex) // ���� �� ����� �������, � ������� ���������� ������ ��� ������� �������.
			{
				for (int j = needIndex; j < AmountCities + needIndex; ++j)                         // �� ������ �� ������� � ������������ �� � �������, ����� ������� �� �����.
					printf("%s -> ", Cities[IndexesCities[needIndex * AmountCities + j]].c_str());
				printf("%s", Cities[IndexesCities[needIndex * (AmountCities + 1)]].c_str());       // ������� �����, � �������� ������� �����������, ����� ������������ �������.
				printf("\n\n����� ������������� ����: %d\n", AllLengths[needIndex]);                // ������� ����� ��������
				break;
			}
		}

		SetConsoleTextAttribute(hConsole, (WORD)LightBlue | (WORD)Black);

		// ������� ��.
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
		printf("���� %s �� ������� ������� ��� ������.\n", TRAVELS_FILE_NAME);
		printf("��������� ������� �����, ��� ������������ � ������� ���� �� ������.\n");
		printf("����� ������������� ���������.\n");
		SetConsoleTextAttribute(hConsole, (WORD)LightBlue | (WORD)Black);
	}

	fclose(*TRAVELS);
	delete TRAVELS;

	Continue();
}

void showWays() // �������� ��������
{
	SetConsoleTextAttribute(hConsole, (WORD)LightBlue | (WORD)Black);
	
	system("cls");
	FILE** TRAVELS = new FILE*;
	if (!fopen_s(TRAVELS, TRAVELS_FILE_NAME, "rb"))
	{
		SetConsoleTextAttribute(hConsole, (WORD)Yellow | (WORD)Black);
		printf("----------------------------------------------------------------\n");
		printf("---------------------------��������-----------------------------\n");
		printf("----------------------------------------------------------------\n");
		SetConsoleTextAttribute(hConsole, (WORD)LightBlue | (WORD)Black);

		// ������ �� ����� ��� ������ � ������� � �������
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
		printf("���� %s �� ������� ������� ��� ������.\n", TRAVELS_FILE_NAME);
		printf("��������� ������� �����, ��� ������������ � ������� ���� �� ������.\n");
		printf("����� ������������� ���������.\n");
		SetConsoleTextAttribute(hConsole, (WORD)LightBlue | (WORD)Black);
	}
	if (*TRAVELS) fclose(*TRAVELS);
	delete TRAVELS;

	Continue();
}

void addWay() // �������� �������.
{
	SetConsoleTextAttribute(hConsole, (WORD)LightBlue | (WORD)Black);
	
	system("cls");
	FILE** TRAVELS = new FILE*;
	if (!fopen_s(TRAVELS, TRAVELS_FILE_NAME, "rb+"))
	{
		SetConsoleTextAttribute(hConsole, (WORD)Yellow | (WORD)Black);
		printf("----------------------------------\n");
		printf("---------����������-������--------\n");
		printf("----------------------------------\n\n");
		SetConsoleTextAttribute(hConsole, (WORD)LightBlue | (WORD)Black);

		std::vector<Trip> AllTrips; // ���� ������� ��� ������� �� �����.
		printf("������� ������ ����������� ������:\n\n");
		// �������� ����������� ������.
		Trip* AddedTrip = new Trip();
		printf("����� �����������: ");
		gets_s(AddedTrip->from, LENGTH_CITY_NAME);
		printf("����� ����������: ");
		gets_s(AddedTrip->to, LENGTH_CITY_NAME);
		printf("����������: ");
		scanf_s("%d", &AddedTrip->distance);
		
		// ������� ������, ������� ��� ������������, � �������� �� (��������������, ��� ���� �����������).
		AllTrips.push_back(*AddedTrip);
		Trip* ReverseAddedTrip = new Trip(AddedTrip->to, AddedTrip->from, AddedTrip->distance);
		AllTrips.push_back(*ReverseAddedTrip);

		delete AddedTrip;
		delete ReverseAddedTrip;
		
		// ������ �� ����� ��� �������.
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

		// ��� ������ ������� ���������� �� STL ��������� ������ ������� � ������������������ ������� �����������.
		std::sort(begin(AllTrips), end(AllTrips), [](const Trip& Obj1, const Trip& Obj2) 
			{
				if (strcmp(Obj1.from, Obj2.from) == 0)
					return strcmp(Obj1.to, Obj2.to) == -1;
				else
					return strcmp(Obj1.from, Obj2.from) == -1;
			});

		// ��������� ������ ��� ������ ���� ��� ������ ���� ������� ������ � �����������.
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
		printf("���� %s �� ������� ������� ��� ��������.\n", TRAVELS_FILE_NAME);
		printf("��������� ������� �����, ��� ������������ � ������� ���� �� ������.\n");
		printf("����� ������������� ���������.\n");
		SetConsoleTextAttribute(hConsole, (WORD)LightBlue | (WORD)Black);
	}

	if(*TRAVELS) fclose(*TRAVELS);
	delete TRAVELS;
	
	Continue();
}

void delWay() // ������� �������.
{
	SetConsoleTextAttribute(hConsole, (WORD)LightBlue | (WORD)Black);

	system("cls");
	FILE** TRAVELS = new FILE*;

	if (!fopen_s(TRAVELS, TRAVELS_FILE_NAME, "rb"))
	{
		// ����������� ������ ������ ��� ��������.
		SetConsoleTextAttribute(hConsole, (WORD)Yellow | (WORD)Black);
		printf("-------------------------------------------------\n");
		printf("-----------------��������-������-----------------\n");
		printf("-------------------------------------------------\n\n");
		SetConsoleTextAttribute(hConsole, (WORD)LightBlue | (WORD)Black);

		printf("������� ������ ������, ������� ��������� �������:\n\n");
		Trip* TripToDelete = new Trip;
		printf("����� �����������: ");
		gets_s(TripToDelete->from, LENGTH_CITY_NAME);
		printf("����� ����������: ");
		gets_s(TripToDelete->to, LENGTH_CITY_NAME);
		printf("����������: ");
		scanf_s("%d", &TripToDelete->distance);
		// ������ ������, �������� ��������, � ���� ������ �� ������ �������, ��������� ����� ����������� ���.
		Trip* reverseTripDelete = new Trip(TripToDelete->to, TripToDelete->from, TripToDelete->distance);

		std::vector<Trip>AllTripsWithoutDeleted; // ���� �������� ��� ������, �� ������� ���������, �� ������ ���������.
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

				// ���������� ������ �� ����� � ��������, ������� ������� ������������� ��� ��������.
				// ���� ������ �� ����������, ����������� � � ������.
				if (TripFromFile->distance != 0 && 
					(strcmp(TripFromFile->from, TripToDelete->from) || strcmp(TripFromFile->to, TripToDelete->to) || TripFromFile->distance != TripToDelete->distance) &&
					(strcmp(TripFromFile->from, reverseTripDelete->from) || strcmp(TripFromFile->to, reverseTripDelete->to) || TripFromFile->distance != reverseTripDelete->distance))
						AllTripsWithoutDeleted.push_back(*TripFromFile);
			}
		}

		// ������������ ����� �������� ����, �������� ��� ������, ����� ��������
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
		printf("���� %s �� ������� ������� ��� ���������.\n", TRAVELS_FILE_NAME);
		printf("��������� ������� �����, ��� ������������ � ������� ���� �� ������.\n");
		printf("����� ������������� ���������.\n");
		SetConsoleTextAttribute(hConsole, (WORD)LightBlue | (WORD)Black);
	}

	if (*TRAVELS) fclose(*TRAVELS);
	delete TRAVELS;

	Continue();
}
