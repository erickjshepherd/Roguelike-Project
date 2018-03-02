#pragma once
#include "Map.h"
#ifndef Console

extern char acceptable[];
void ClearScreen();
void Draw_Console(Map* map, int Size);
bool Acceptable(char check);

#endif