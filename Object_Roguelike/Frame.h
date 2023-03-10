#pragma once
#include "Shared.h"
#include "Texture.h"

class Frame {
public:
	Frame();
	Frame(Texture* sheet_in, SDL_Color color_in, int sheetIndex_in = 0, int xOff_in = 0, int yOff_in = 0);
	~Frame();

private:
	Texture* sheet;
	int sheetIndex;
	SDL_Color color;
	int xOff;
	int yOff;
};
