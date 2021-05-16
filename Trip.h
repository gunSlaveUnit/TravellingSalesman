 // Содержит структуру поездки.

#pragma once

#ifndef _TRIP_H_
#define _TRIP_H_

#include <string>      // Для strcpy_s в Trip.cpp.

#include "Constants.h" // Константы, здесь требуется LENGHT_CITY_NAME.

struct Trip // Информация о поездке.
{
	Trip();                        // Конструтор по умолчанию.
	Trip(char* f, char* t, int d); // Конструктор с параметрами.
	Trip(const Trip& Obj);		   // Конструктор копирования.

	char from[LENGTH_CITY_NAME]; // Пункт отправления. 
	char to[LENGTH_CITY_NAME];   // Пункт назначения. 
	int distance;				 // Расстояние между пунктами.
};

#endif // !_TRIP_H_
