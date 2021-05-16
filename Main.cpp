// ���� Main.cpp �������� ����� ����� � ��������� - ����� ���������� � ������������� ���������� ���������.

/*
* ��������� ��������������� � ������������ ������� Windows 10 x64, IDE - Microsoft Visual Studio 2019.
*/

/*
* 6. ����: ���������� ��������� ����������� ������������ ��������.
*
* ������ ��������� ��� ������ ��������� ���������� ������.
* ������ �������� �������� ������ (�������� �� ������ ������ ����������� ����������� ��������).
*
* �������� ��� ������ ��-�� ������������� �������� ���������� � ������������ ������ (��������� �������� ������ ����� � ������),  
* � �� ����� ��� ����������� ������ ���������� (��������, �������� �������� ��� �������� ��������-�����) ������� ���� ���������� ���� �� ����� �� �����, 
* ���� ���������� ���� �� ����� �� ���� ������ ����� (���� �� ���� ����� ������������).
*
* �� ����� ���� ������ ������� � ���������� ������������ ����� - ������������ ��������, ����������� ����� ������ ������� ������ ���� ���.
*
* �������� �������� ���������� �������, ������� �������� �� ������������ ������� �������.
* ������ ���������� �������� ��������� ���������, ������� ��������� ��������� ���������� ������� ����� � ����������,
* ������ ��� ������� ���� �� ���������� ������� ���������� �������.
* ���������� ������ ��������� � ��������� ���������� � ����� MenuItems.cpp.
*
* ������, ������������� � ����� travels.bin, ���� � https://ru.wikipedia.org/wiki/%D0%97%D0%B0%D0%B4%D0%B0%D1%87%D0%B0_%D0%BA%D0%BE%D0%BC%D0%BC%D0%B8%D0%B2%D0%BE%D1%8F%D0%B6%D1%91%D1%80%D0%B0
* ������� ������: �������,�������,ʸ���,�������,������,�����,������,��������,��������,��������,��������,���������-��-�����,�����������,������
* � ������� ���� ������� 14 (�� �������� � ��������� �� ������� ����� 15 �������), �������, ����������� ����������, ����� �� ������� �� ��������.
* ���������� ���� ����� � https://www.avtodispetcher.ru/distance/, ����� ���������� �� ������ (� ������ ����� ������������� �����).
*/

#include "Headers.h" // �������� ��� ������������ ������������ �����.

extern HANDLE hConsole; // ��������� ��� extern ��� ������������� � ������ ������.

void SetLanguage() // ��������� ��������� Windows-1251 � ����� ����� � ������ ��� ����������� �������� ������.
{
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);
}

void SetTypeLucidaConsole() // ������ ����� Lucida Console ��� ������ � ���������� Windows-1251.
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
	SetLanguage();                                     // ��������� ��������� Windows-1251 ��� ����������� �������� ������.
	SetTypeLucidaConsole();                            // ������ ����� Lucida Console ��� ������ � ���������� Windows-1251.
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE); // ��������� ���������� � ���������� ����.
	HWND hcon = GetConsoleWindow();                    // ��������� ���������� � ���������� ����.
	MoveWindow(hcon, 100, 10, 1280, 1024, FALSE);      // ���� ����������� �� ����������� (100;10) �� ������ �������� ����, ������� 1280�1024.

	COORD position[NUMBER_MENU_ITEMS]; // ���������� ������� ����.
	int selectedItem = 0;              // ��������� ����� ����.          
	do
	{
		int xmax, ymax;

		// ��������� ���������� ����. 
		PCONSOLE_SCREEN_BUFFER_INFO pwi = new CONSOLE_SCREEN_BUFFER_INFO;
		PWINDOWINFO pgwi = new WINDOWINFO;
		pgwi = nullptr;
		GetConsoleScreenBufferInfo(hConsole, pwi);
		if(pgwi) GetWindowInfo(hcon, pgwi);
		xmax = pwi->dwSize.X;
		ymax = pwi->dwSize.Y;

		// ���������� ��������� ������� ����.
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
		SetConsoleCursorPosition(hConsole, position[selectedItem]);	                           // ��������� �������� ������ ����� ������
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
			getDirections(); // ���������� ��������.
			break;
		case 1:
			showWays();      // �������� ��������.
			break;
		case 2:
			addWay();        // �������� �������.
			break;
		case 3:
			delWay();        // ������� �������.
		}
	} while (selectedItem != 4);

	return 0;
}
