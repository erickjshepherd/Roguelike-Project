#pragma once
#include "Map.h"
#ifndef Console

extern char acceptable[];
void ClearScreen();
void Draw_Console(Map* map, int Size);
void ClearLine(COORD position, int num);
bool Acceptable(char check);

#endif