// Содержит описание методов структуры Trip.

#include "Trip.h" // Структура поездки.

Trip::Trip() // Конструтор по умолчанию.
{
	strcpy_s(from, LENGTH_CITY_NAME, "");
	strcpy_s(to, LENGTH_CITY_NAME, "");
	distance = 0;
}

Trip::Trip(char* f, char* t, int d) // Конструктор с параметрами.
{
	strcpy_s(from, LENGTH_CITY_NAME, f);
	strcpy_s(to, LENGTH_CITY_NAME, t);
	distance = d;
}

Trip::Trip(const Trip& Obj) // Конструктор копирования.
{
	strcpy_s(from, LENGTH_CITY_NAME, Obj.from);
	strcpy_s(to, LENGTH_CITY_NAME, Obj.to);
	distance = Obj.distance;
}
