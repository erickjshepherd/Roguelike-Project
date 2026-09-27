#include "stdafx.h"
#include "SDLFuncs.h"
#include "Texture.h"

SDL_Window* window_g;
SDL_Renderer* renderer_g;
std::vector<TTF_Font*> fonts_g;
SDL_Rect mapView_g;
SDL_Rect statsView_g;
SDL_Rect eventsView_g;
SDL_Color textColor_g;
int tileSize_g;
int currentFrame_g;
int drawFrame_g;

int SDL_Init() {
	
	window_g = NULL;

	// Initialise SDL
	if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
		exit(-1);
	}

	// Create our window
	window_g = SDL_CreateWindow("Untitled Roguelike", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);

	// Make sure creating the window succeeded
	if (!window_g) {
		system("pause");
		return 1;
	}

	//Initialize PNG loading
	int imgFlags = IMG_INIT_PNG;
	if (!(IMG_Init(imgFlags) & imgFlags))
	{
		return 1;
	}

	//Initialize SDL_ttf
	if (TTF_Init() == -1)
	{
		return 1;
	}

	// set up the text color
	textColor_g.r = 255;
	textColor_g.g = 255;
	textColor_g.b = 255;

	// load the fonts
	loadFonts();

	// set up the renderer and viewports
	renderer_g = NULL;
	rendererInit();

	// set up the frame clock
	currentFrame_g = 0;
	drawFrame_g = 0;

	return 0;
}

bool loadImage(SDL_Texture* &image, std::string path) {
	SDL_Surface* tempSurface = IMG_Load(path.c_str());
	if (tempSurface == NULL) {
		return 1;
	}
	else {
		image = SDL_CreateTextureFromSurface(renderer_g, tempSurface);
		SDL_FreeSurface(tempSurface);
		if (image == NULL) {
			return 1;
		}
	}
}

void SDL_Close() {
	freeTilesets();
	freeFonts();
	
	SDL_DestroyRenderer(renderer_g);
	renderer_g = NULL;
	SDL_DestroyWindow(window_g);
	window_g = NULL;

	TTF_Quit();
	IMG_Quit();
	SDL_Quit();
}

int handleEvents() {
	/* Poll for events */
	SDL_Event event;
	while (SDL_PollEvent(&event)) {
		if (event.type == SDL_QUIT) {
			return EVENT_QUIT;
		}
		else if (event.type == SDL_KEYDOWN) {
			switch (event.key.keysym.sym) {
			case SDLK_LEFT:
				return EVENT_KEY_LEFT;
			case SDLK_RIGHT:
				return EVENT_KEY_RIGHT;
			case SDLK_UP:
				return EVENT_KEY_UP;
			case SDLK_DOWN:
				return EVENT_KEY_DOWN;
			case SDLK_1:
				return EVENT_KEY_1;
			case SDLK_2:
				return EVENT_KEY_2;
			case SDLK_3:
				return EVENT_KEY_3;
			case SDLK_RETURN:
				return EVENT_KEY_ENTER;
			case SDLK_ESCAPE:
				return EVENT_KEY_ESC;
			case SDLK_TAB:
				return EVENT_KEY_TAB;
			}
		}
		else if (event.type == SDL_WINDOWEVENT) {
			// SIZE_CHANGED fires for every size change, including fullscreen
			// switches and SDL_SetWindowSize; RESIZED only fires for user drags
			if (event.window.event == SDL_WINDOWEVENT_SIZE_CHANGED) {
				rendererInit();
				return EVENT_RESIZE;
			}
		}
		else if (event.type == SDL_RENDER_DEVICE_RESET) {
			// the GPU device was lost: every texture is invalid and must be rebuilt
			reloadTextures();
			return EVENT_RESIZE;
		}
	}
	return -1;
}

void loadFonts() {
	TTF_Font* font_8 = TTF_OpenFont("source/Dawnlike Tiles/GUI/SDS_8x8.ttf", 8);
	fonts_g.push_back(font_8);
	TTF_Font* font_16 = TTF_OpenFont("source/Dawnlike Tiles/GUI/SDS_8x8.ttf", 16);
	fonts_g.push_back(font_16);
	TTF_Font* font_24 = TTF_OpenFont("source/Dawnlike Tiles/GUI/SDS_8x8.ttf", 24);
	fonts_g.push_back(font_24);
	TTF_Font* font_32 = TTF_OpenFont("source/Dawnlike Tiles/GUI/SDS_8x8.ttf", 32);
	fonts_g.push_back(font_32);
}

void freeFonts() {
	while (!fonts_g.empty()) {
		TTF_CloseFont(fonts_g.back());
		fonts_g.pop_back();
	}
}

void rendererInit() {
	// Create the renderer once. An SDL2 renderer survives window resizes and
	// fullscreen switches, and every texture created from it stays valid, so
	// it is never torn down and rebuilt here. Mixing the window-surface API
	// (SDL_GetWindowSurface / SDL_UpdateWindowSurface) with a renderer on the
	// same window is unsupported and froze the display after going fullscreen,
	// so this function no longer touches the window surface at all.
	bool firstInit = (renderer_g == NULL);
	if (firstInit) {
		renderer_g = SDL_CreateRenderer(window_g, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
		if (renderer_g == NULL) {
			SDL_Log("SDL_CreateRenderer failed: %s", SDL_GetError());
			// fall back to whatever SDL can give us
			renderer_g = SDL_CreateRenderer(window_g, -1, 0);
			if (renderer_g == NULL) {
				SDL_Log("SDL_CreateRenderer fallback failed: %s", SDL_GetError());
				exit(-1);
			}
		}
		SDL_SetRenderDrawColor(renderer_g, 0, 0, 0, 255);
	}

	// get the screen dimensions
	int screenW, screenH;
	SDL_GetWindowSize(window_g, &screenW, &screenH);

	// set the tile size to be the largest multiple of 16 that fits on the screen
	int smallestDim = (screenH < screenW) ? screenH : screenW;
	int tileScale = (smallestDim / MAX_MAP_SIZE) / TILE_SOURCE_SIZE;
	if (tileScale == 0) {
		tileScale = 1;
		// only grow a windowed window: resizing a fullscreen window is a no-op
		// and would just queue another size-changed event
		if (!(SDL_GetWindowFlags(window_g) & SDL_WINDOW_FULLSCREEN_DESKTOP)) {
			SDL_SetWindowSize(window_g, TILE_SOURCE_SIZE * MAX_MAP_SIZE, TILE_SOURCE_SIZE * MAX_MAP_SIZE);
			SDL_GetWindowSize(window_g, &screenW, &screenH);
		}
	}
	tileSize_g = TILE_SOURCE_SIZE * tileScale;
	int mapSize = tileSize_g * MAX_MAP_SIZE;

	// update the viewports
	int textSpace = (getTextSpace() * 3) / 2;
	mapView_g.x = (screenH - mapSize) / 2;
	mapView_g.y = (screenH - mapSize) / 2;
	mapView_g.w = mapSize;
	mapView_g.h = mapSize;
	statsView_g.x = screenH + (tileSize_g / 2);
	statsView_g.y = (tileSize_g / 2);
	statsView_g.w = (screenW - screenH) - tileSize_g;
	statsView_g.h = (NUM_STAT_LINES + 1) * textSpace - tileSize_g;
	eventsView_g.x = screenH + (tileSize_g / 2);
	eventsView_g.y = (NUM_STAT_LINES + 1) * textSpace + (tileSize_g / 2);
	eventsView_g.w = (screenW - screenH) - tileSize_g;
	eventsView_g.h = (screenH - ((NUM_STAT_LINES + 1) * textSpace)) - tileSize_g;

	// start the next frame from a clean, full-window backbuffer
	SDL_RenderSetViewport(renderer_g, NULL);
	SDL_RenderClear(renderer_g);

	// load the tilesets once; they belong to the renderer and stay valid across
	// resizes. reloadTextures() rebuilds them if the GPU device is ever lost.
	if (firstInit) {
		loadTileSets();
		loadTileSets2();
	}
}

// Rebuild every texture. Only needed when SDL reports SDL_RENDER_DEVICE_RESET,
// which invalidates all existing textures.
void reloadTextures() {
	freeTilesets();
	loadTileSets();
	loadTileSets2();
}

int getTextSpace() {
	int textIndex = (tileSize_g / 16) - 1;
	if (textIndex >= NUM_FONTS) {
		textIndex = NUM_FONTS - 1;
	}
	int h = TTF_FontHeight(fonts_g[textIndex]);
	return h;
}

// Advance the passive animation clock.
// The current frame is derived from SDL's millisecond timer, so it is a pure
// function of elapsed time: no thread, no locking, and it can never drift.
// Call this once per iteration of any loop that renders the map.
void updateFrameClock() {
	currentFrame_g = (SDL_GetTicks() / FRAME_WAIT) % ANIMATION_FRAMES;
}

int inputEventFilter(void* data, SDL_Event* event) {
	if (event->type == SDL_KEYDOWN || event->type == SDL_KEYUP || event->type == SDL_MOUSEMOTION) {
		return 0;
	}
	return 1;
}

int noFilter(void* data, SDL_Event* event) {
	return 1;
}

void filterInputEvents() {
	SDL_SetEventFilter(inputEventFilter, nullptr);
}

void resetFilter() {
	SDL_SetEventFilter(noFilter, nullptr);
}

void clearEvents() {
	SDL_Event event;
	while (SDL_PollEvent(&event)) {
	
	}
}

// does the input need to be handled by the player object
bool validPlayerInput(int input) {
	bool valid;
	if (input == EVENT_KEY_UP) {
		valid = 1;
	}
	else if (input == EVENT_KEY_DOWN) {
		valid = 1;
	}
	else if (input == EVENT_KEY_LEFT) {
		valid = 1;
	}
	else if (input == EVENT_KEY_RIGHT) {
		valid = 1;
	}
	else if (input == EVENT_KEY_1) {
		valid = 1;
	}
	else if (input == EVENT_KEY_2) {
		valid = 1;
	}
	else if (input == EVENT_KEY_3) {
		valid = 1;
	}
	else if (input == EVENT_KEY_ENTER) {
		valid = 1;
	}
	else if (input == EVENT_KEY_TAB) {
		valid = 1;
	}
	else {
		valid = 0;
	}
	return valid;
}
