
#include "Texture.h"

std::string tilePaths[NUMPATHS] = {
	"Dawnlike Tiles/Objects/Tile.png",
	"Dawnlike Tiles/Objects/Wall.png",
	"Dawnlike Tiles/Characters/Player0.png",
	"Dawnlike Tiles/Characters/Slime0.png",
	"Dawnlike Tiles/Items/MedWep.png",
	"Dawnlike Tiles/Items/Scroll.png",
	"Dawnlike Tiles/Items/ShortWep.png",
	"Dawnlike Tiles/Objects/Floor.png",
	"Dawnlike Tiles/GUI/GUI0.png"
};

std::string tilePaths2[NUMPATHS] = {
	"",
	"",
	"Dawnlike Tiles/Characters/Player1.png",
	"Dawnlike Tiles/Characters/Slime1.png",
	"",
	"",
	"",
	"",
	""
};

std::vector<Texture*> tileSets_g;
std::vector<Texture*> tileSets2_g;

Texture::Texture() {
	width = 0;
	height = 0;
	texture = NULL;
}

Texture::~Texture() {
	free();
}

// todo: memory leak here?
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

bool Texture::loadFromRenderedText(std::string textureText, SDL_Color textColor, int font)
{
	//Get rid of preexisting texture
	free();

	//Render text surface
	int textIndex = (tileSize_g / 16) - 1;
	if (textIndex >= NUM_FONTS) {
		textIndex = NUM_FONTS - 1;
	}
	// automatically set font if input is -1
	if (font == -1) {
		font = textIndex;
	}
	SDL_Surface* textSurface = TTF_RenderText_Solid(fonts_g[font], textureText.c_str(), textColor);
	if (textSurface == NULL)
	{
		printf("Unable to render text surface! SDL_ttf Error: %s\n", TTF_GetError());
	}
	else
	{
		//Create texture from surface pixels
		texture = SDL_CreateTextureFromSurface(renderer_g, textSurface);
		if (texture == NULL)
		{
			printf("Unable to create texture from rendered text! SDL Error: %s\n", SDL_GetError());
		}
		else
		{
			//Get image dimensions
			width = textSurface->w;
			height = textSurface->h;
		}

		//Get rid of old surface
		SDL_FreeSurface(textSurface);
	}

	//Return success
	return texture != NULL;
}

void Texture::setColor(Uint8 red, Uint8 green, Uint8 blue) {
	//Modulate texture
	SDL_SetTextureColorMod(texture, red, green, blue);
}

void Texture::render(int x, int y, SDL_Rect* clip = NULL) {
	SDL_Rect renderQuad = { x, y, width, height };

	int imageScale = tileSize_g / TILE_SOURCE_SIZE;

	if (clip != NULL) {
		renderQuad.w = clip->w * imageScale;
		renderQuad.h = clip->h * imageScale;
	}

	SDL_RenderCopy(renderer_g, texture, clip, &renderQuad);
}

int Texture::getWidth() {
	return width;
}

int Texture::getHeight() {
	return height;
}

SDL_Texture* Texture::getTexture() {
	return texture;
}

// todo: maybe load textures into a 2D array and only load the ones that are used.
// todo: merge the load functions
// The enemy tileset is the main one that needs these colors
void loadTileSets() {
	int x;
	for (x = 0; x < NUMPATHS; x++) {
		// standard tiles
		Texture* tileSet = new Texture();
		tileSet->loadFromFile(tilePaths[x]);
		tileSets_g.push_back(tileSet);

		// cast preview tiles
		Texture* tileSetCast = new Texture();
		tileSetCast->loadFromFile(tilePaths[x]);
		tileSetCast->setColor(CASTR, CASTG, CASTB);
		tileSets_g.push_back(tileSetCast);

		// red tiles
		Texture* tileSetRed = new Texture();
		tileSetRed->loadFromFile(tilePaths[x]);
		tileSetRed->setColor(255, 0, 0);
		tileSets_g.push_back(tileSetRed);

		// cyan tiles
		Texture* tileSetCyan = new Texture();
		tileSetCyan->loadFromFile(tilePaths[x]);
		tileSetCyan->setColor(102, 255, 255);
		tileSets_g.push_back(tileSetCyan);

		// orange tiles
		Texture* tileSetOrange = new Texture();
		tileSetOrange->loadFromFile(tilePaths[x]);
		tileSetOrange->setColor(255, 93, 0);
		tileSets_g.push_back(tileSetOrange);

		// dark green tiles
		Texture* tileSetDGreen = new Texture();
		tileSetDGreen->loadFromFile(tilePaths[x]);
		tileSetDGreen->setColor(51, 153, 51);
		tileSets_g.push_back(tileSetDGreen);

		// purple tiles
		Texture* tileSetPurple = new Texture();
		tileSetPurple->loadFromFile(tilePaths[x]);
		tileSetPurple->setColor(153, 0, 255);
		tileSets_g.push_back(tileSetPurple);

		// pink tiles
		Texture* tileSetPink = new Texture();
		tileSetPink->loadFromFile(tilePaths[x]);
		tileSetPink->setColor(255, 102, 204);
		tileSets_g.push_back(tileSetPink);
	}
}

// loads tilesets for the other frame of animation
void loadTileSets2() {
	int x;
	for (x = 0; x < NUMPATHS; x++) {
		// standard tiles
		Texture* tileSet = new Texture();
		tileSet->loadFromFile(tilePaths2[x]);
		tileSets2_g.push_back(tileSet);

		// cast preview tiles
		Texture* tileSetCast = new Texture();
		tileSetCast->loadFromFile(tilePaths2[x]);
		tileSetCast->setColor(CASTR, CASTG, CASTB);
		tileSets2_g.push_back(tileSetCast);

		// red tiles
		Texture* tileSetRed = new Texture();
		tileSetRed->loadFromFile(tilePaths2[x]);
		tileSetRed->setColor(255, 0, 0);
		tileSets2_g.push_back(tileSetRed);

		// cyan tiles
		Texture* tileSetCyan = new Texture();
		tileSetCyan->loadFromFile(tilePaths2[x]);
		tileSetCyan->setColor(102, 255, 255);
		tileSets2_g.push_back(tileSetCyan);

		// orange tiles
		Texture* tileSetOrange = new Texture();
		tileSetOrange->loadFromFile(tilePaths2[x]);
		tileSetOrange->setColor(255, 93, 0);
		tileSets2_g.push_back(tileSetOrange);

		// dark green tiles
		Texture* tileSetDGreen = new Texture();
		tileSetDGreen->loadFromFile(tilePaths2[x]);
		tileSetDGreen->setColor(51, 153, 51);
		tileSets2_g.push_back(tileSetDGreen);

		// purple tiles
		Texture* tileSetPurple = new Texture();
		tileSetPurple->loadFromFile(tilePaths2[x]);
		tileSetPurple->setColor(153, 0, 255);
		tileSets2_g.push_back(tileSetPurple);

		// pink tiles
		Texture* tileSetPink = new Texture();
		tileSetPink->loadFromFile(tilePaths2[x]);
		tileSetPink->setColor(255, 102, 204);
		tileSets2_g.push_back(tileSetPink);
	}
}

void freeTilesets() {
	while (!tileSets_g.empty()) {
		delete tileSets_g.back();
		tileSets_g.pop_back();
	}
	while (!tileSets2_g.empty()) {
		delete tileSets2_g.back();
		tileSets2_g.pop_back();
	}
}
