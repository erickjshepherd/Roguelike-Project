#pragma once

#include <string>
#include "SDLFuncs.h"

class Texture {
public:
	Texture();
	~Texture();
	bool loadFromFile(std::string path);
	void free();
	void render(int x, int y, SDL_Rect* clip);
	int getWidth();
	int getHeight();

private:
	SDL_Texture* texture;
	int width;
	int height;
};