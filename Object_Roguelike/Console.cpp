#pragma once
#include <iostream>
#include <Windows.h>
#include "stdafx.h"
#include <Windows.h>
#include <time.h>
#include <cstdlib>
#include <ctime>
#include <string>
#include <iostream>
#include "Map.h"
#include "Player.h"
#include "Console.h"

char acceptable[] = { '#', '.', '@', 'E', 'S', 'd', '$' };

void Draw_Console(Map* map, int Size) {

	int x;
	int y;
	int xy;

	for (y = 0; y < Size; y++) {

		for (x = 0; x < Size; x++) {

			xy = y * Size;
			xy += x;

			std::cout << map->map[xy];
		}

		std::cout << std::endl;
	}
}

void ClearScreen()
{
	HANDLE                     hStdOut;
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	DWORD                      count;
	DWORD                      cellCount;
	COORD                      homeCoords = { 0, 0 };

	hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
	if (hStdOut == INVALID_HANDLE_VALUE) return;

	/* Get the number of cells in the current buffer */
	if (!GetConsoleScreenBufferInfo(hStdOut, &csbi)) return;
	cellCount = csbi.dwSize.X *csbi.dwSize.Y;

	/* Fill the entire buffer with spaces */
	if (!FillConsoleOutputCharacter(
		hStdOut,
		(TCHAR) ' ',
		cellCount,
		homeCoords,
		&count
	)) return;

	/* Fill the entire buffer with the current colors and attributes */
	if (!FillConsoleOutputAttribute(
		hStdOut,
		csbi.wAttributes,
		cellCount,
		homeCoords,
		&count
	)) return;

	/* Move the cursor home */
	SetConsoleCursorPosition(hStdOut, homeCoords);
}

// todo: make this actually clear a line, not just write a set number of spaces
void ClearLine(COORD position, int num) {
	HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleCursorPosition(handle, position);
	int x;
	for (x = 0; x < num; x++) {
		std::cout << " ";
	}
}

bool Acceptable(char check) {

	int x;

	for (x = 0; x < 100; x++) {
	
		if (check == acceptable[x]) {
		
			return true;
		}
	}

	return false;
}
