 // ???????? ????????? ???????.

#pragma once

#ifndef _TRIP_H_
#define _TRIP_H_

#include <string>      // ??? strcpy_s ? Trip.cpp.

#include "Constants.h" // ?????????, ????? ????????? LENGHT_CITY_NAME.

struct Trip // ?????????? ? ???????.
{
	Trip();                        // ?????????? ?? ?????????.
	Trip(char* f, char* t, int d); // ??????????? ? ???????????.
	Trip(const Trip& Obj);		   // ??????????? ???????????.

	char from[LENGTH_CITY_NAME]; // ????? ???????????. 
	char to[LENGTH_CITY_NAME];   // ????? ??????????. 
	int distance;				 // ?????????? ????? ????????.
};

#endif // !_TRIP_H_
