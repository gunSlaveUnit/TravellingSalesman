// Файл Main.cpp содержит точку входа в программу - здесь начинается и заканчивается выполнение программы.

/*
* Программа компилировалась в операционной система Windows 10 x64, IDE - Microsoft Visual Studio 2019.
*/

/*
* 6. Тема: Разработка программы определения оптимального маршрута.
*
* Задача выполнена при помощи алгоритма ближайшего соседа.
* Данный алгоритм является жадным (означает не всегда точное определение кратчайшего маршрута).
*
* Алгоритм был выбран из-за относительной простоты реализации и формулировки задачи (требуется обходить каждый город в списке),  
* в то время как большинство других алгоритмов (например, алгоритм Дейкстры или алгоритм Беллмана-Форда) находят либо кратчайший путь от точки до точки, 
* либо кратчайший путь от точки до всех других точек (хотя их тоже можно приспособить).
*
* По факту суть задачи состоит в нахождении гамильтонова цикла - замкнутового маршрута, проходящего через каждую вершину только один раз.
*
* Алгоритм является достаточно быстрым, неплохо работает на симметричной матрице городов.
* Данная реализация содержит небольшое улучшение, которое позволяет алгоритму определять маршрут ближе к идеальному,
* однако это заметно лишь на достаточно большом количестве городов.
* Объяснение работы алгоритма и улучшения содержится в файле MenuItems.cpp.
*
* Пример, расположенный в файле travels.bin, взят с https://ru.wikipedia.org/wiki/%D0%97%D0%B0%D0%B4%D0%B0%D1%87%D0%B0_%D0%BA%D0%BE%D0%BC%D0%BC%D0%B8%D0%B2%D0%BE%D1%8F%D0%B6%D1%91%D1%80%D0%B0
* Главный пример: Гамбург,Дрезден,Кёльн,Лейпциг,Мюнхен,Эссен,Берлин,Нюрнберг,Ганновер,Штутгарт,Дуйсбург,Франкфурт-на-Майне,Дюссельдорф,Бремен
* В примере выше городов 14 (на картинке в Википедии не удалось найти 15 городов), маршрут, построенный программой, похож на маршрут на картинке.
* Расстояния были взяты с https://www.avtodispetcher.ru/distance/, взяты расстояния по трассе (в задаче карта автомобильных дорог).
*/

#include "Headers.h" // Содержит все подключаемые заголовочные файлы.

extern HANDLE hConsole; // Объявляем как extern для использования в других местах.

void SetLanguage() // Установка кодировки Windows-1251 в поток ввода и вывода для отображения русского текста.
{
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);
}

void SetTypeLucidaConsole() // Ставим шрифт Lucida Console для работы с кодировкой Windows-1251.
{
	HANDLE hCon = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
	CONSOLE_FONT_INFOEX cfi;
	if (hCon != INVALID_HANDLE_VALUE) {
		GetCurrentConsoleFontEx(hConsole, TRUE, &cfi);
		cfi.cbSize = sizeof(cfi);
		cfi.nFont = 0;
		cfi.dwFontSize.X = 0;
		cfi.dwFontSize.Y = 20;
		cfi.FontFamily = FF_DONTCARE;
		cfi.FontWeight = FW_NORMAL;
		wcscpy_s(cfi.FaceName, L"Lucida Console");
		SetCurrentConsoleFontEx(hConsole, FALSE, &cfi);
	}
}

int main()
{
	SetLanguage();                                     // Установка кодировки Windows-1251 для отображения русского текста.
	SetTypeLucidaConsole();                            // Ставим шрифт Lucida Console для работы с кодировкой Windows-1251.
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE); // Получение информации о консольном окне.
	HWND hcon = GetConsoleWindow();                    // Получение информации о консольном окне.
	MoveWindow(hcon, 100, 10, 1280, 1024, FALSE);      // Окно распологаем по координатам (100;10) от левого верхнего угла, размеры 1280Х1024.

	COORD position[NUMBER_MENU_ITEMS]; // Координаты пунктов меню.
	int selectedItem = 0;              // Выбранный пункт меню.          
	do
	{
		int xmax, ymax;

		// Получение параметров окна. 
		PCONSOLE_SCREEN_BUFFER_INFO pwi = new CONSOLE_SCREEN_BUFFER_INFO;
		PWINDOWINFO pgwi = new WINDOWINFO;
		pgwi = nullptr;
		GetConsoleScreenBufferInfo(hConsole, pwi);
		if(pgwi) GetWindowInfo(hcon, pgwi);
		xmax = pwi->dwSize.X;
		ymax = pwi->dwSize.Y;

		// Определяем положение пунктов меню.
		int y0 = 7;
		for (int i = 0; i < NUMBER_MENU_ITEMS; ++i)
		{
			position[i].X = (xmax - strlen(ITEMS[i])) / 2;
			position[i].Y = y0 + i;
		}
		system("cls");			
		SetConsoleTextAttribute(hConsole, (WORD)Yellow| (WORD) Black); 
		
		COORD pos;
		pos.X = (xmax - strlen(TITLE)) / 2;
		pos.Y = 5;
		SetConsoleCursorPosition(hConsole, pos);
		puts(TITLE);
		SetConsoleTextAttribute(hConsole, (WORD)LightBlue | (WORD)Black);
		for (int i = 0; i < NUMBER_MENU_ITEMS; ++i)
		{
			SetConsoleCursorPosition(hConsole, position[i]);
			puts(ITEMS[i]);
		}
		SetConsoleTextAttribute(hConsole, (WORD)Magenta | FOREGROUND_INTENSITY | (WORD)Black);
		SetConsoleCursorPosition(hConsole, position[selectedItem]);	                           // выделение текущего пункта ярким цветом
		puts(ITEMS[selectedItem]);
		unsigned char pressedKey;
		do
		{
			pressedKey = _getch();
			if (pressedKey == 224)
			{
				pressedKey = _getch();
				switch (pressedKey)
				{
				case 72:
					SetConsoleCursorPosition(hConsole, position[selectedItem]);
					SetConsoleTextAttribute(hConsole, (WORD)LightBlue | (WORD)Black);
					puts(ITEMS[selectedItem]);
					--selectedItem;
					if (selectedItem < 0) selectedItem = 4;
					SetConsoleTextAttribute(hConsole, (WORD)Magenta | FOREGROUND_INTENSITY | (WORD)Black);
					SetConsoleCursorPosition(hConsole, position[selectedItem]);
					puts(ITEMS[selectedItem]); break;
				case 80:
					SetConsoleCursorPosition(hConsole, position[selectedItem]);
					SetConsoleTextAttribute(hConsole, (WORD)LightBlue | (WORD)Black);
					puts(ITEMS[selectedItem]);
					++selectedItem;
					if (selectedItem > 4) selectedItem = 0;
					SetConsoleTextAttribute(hConsole, (WORD)Magenta | FOREGROUND_INTENSITY | (WORD)Black);
					SetConsoleCursorPosition(hConsole, position[selectedItem]);
					puts(ITEMS[selectedItem]); break;
				}
			}
		} while (pressedKey != 13);
		switch (selectedItem)
		{
		case 0:
			getDirections(); // Построение маршрута.
			break;
		case 1:
			showWays();      // Показать маршруты.
			break;
		case 2:
			addWay();        // Добавить маршрут.
			break;
		case 3:
			delWay();        // Удалить маршрут.
		}
	} while (selectedItem != 4);

	return 0;
}
