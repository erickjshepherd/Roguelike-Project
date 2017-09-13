// Roguelike.cpp : Defines the entry point for the console application.
//

#include <Windows.h>
#include <time.h>
#include <cstdlib>
#include <ctime>
#include <string>
#include <iostream>
#include "MapGen.h"

using namespace std;

void ClearScreen() {
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

void Draw_Console(char* map) {

	int x;
	int y;
	int xy;

	for (y = 0; y < Size; y++) {
	
		for (x = 0; x < Size; x++) {
		
			xy = y * Size;
			xy += x;
			
			cout << map[xy];
		}
		
		cout << endl;
	}
}

int main() {
	//FreeConsole();
	int x = 0;
	srand((unsigned)time(0));

	char* map;
	map = Map_Generate();

	Draw_Console(map);
	
	while(1) {
	
	}
	
	delete map;

	return 0;
}

