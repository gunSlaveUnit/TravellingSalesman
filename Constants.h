// Содержит основные константы.

#pragma once

#ifndef _CONSTANTS_H_
#define _CONSTANTS_H_

static const char* TRAVELS_FILE_NAME = "travels.bin";           // Имя файла с маршрутами.   
static const char* TITLE             = "Путешествие";           // Заголовок.
const int   NUMBER_MENU_ITEMS        = 5;			            // Количество пунктов меню.
const int   LENGTH_CITY_NAME         = 20;                      // Максимальная длина имени города.
const char  ITEMS[5][20]             = { "Построение маршрута", // Наименования пунктов меню.            
								         "Просмотр маршрутов",
										 "Добавление маршрута",
										 "Удаление маршрута",
										 "Выход" };

#endif // !_CONSTANTS_H_
