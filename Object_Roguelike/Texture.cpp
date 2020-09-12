
#include "Texture.h"

std::vector<Texture*> tileSets_g;

Texture::Texture() {
	width = 0;
	height = 0;
	texture = NULL;
}

Texture::~Texture() {
	free();
}

void Texture::free() {
	if (texture != NULL) {
		SDL_DestroyTexture(texture);
		texture = NULL;
		width = 0;
		height = 0;
	}
}

bool Texture::loadFromFile(std::string path) {
	SDL_Surface* tempSurface = IMG_Load(path.c_str());
	if (tempSurface == NULL) {
		return 1;
	}
	else {
		//SDL_SetColorKey(tempSurface, SDL_TRUE, SDL_MapRGB(tempSurface->format, 0, 0xFF, 0xFF));
		texture = SDL_CreateTextureFromSurface(renderer_g, tempSurface);
		if (texture == NULL) {
			return 1;
		}
		else {
			width = tempSurface->w;
			height = tempSurface->h;
		}
		SDL_FreeSurface(tempSurface);
	}
}

void Texture::render(int x, int y, SDL_Rect* clip = NULL) {
	SDL_Rect renderQuad = { x, y, width, height };

	if (clip != NULL) {
		renderQuad.w = clip->w;
		renderQuad.h = clip->h;
	}

	SDL_RenderCopy(renderer_g, texture, clip, &renderQuad);
}

int Texture::getWidth() {
	return width;
}

int Texture::getHeight() {
	return height;
}

void loadTileSets() {

	int x;
	for (x = 0; x < NUMPATHS; x++) {
		Texture* tileSet = new Texture();
		tileSet->loadFromFile(tilePaths[x]);
		tileSets_g.push_back(tileSet);
	}
}
