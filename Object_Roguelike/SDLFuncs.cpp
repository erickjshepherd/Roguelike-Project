#include "SDLFuncs.h"

SDL_Surface* winSurface_g;
SDL_Window* window_g;
SDL_Renderer* renderer_g;
std::vector<TTF_Font*> fonts_g;
SDL_Rect mapView_g;
SDL_Rect statsView_g;
SDL_Rect infoView_g;
SDL_Color textColor_g;

int SDL_Init() {
	
	winSurface_g = NULL;
	window_g = NULL;

	// Initialise SDL
	if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
		exit(-1);
	}

	// Create our window
	window_g = SDL_CreateWindow("Untitled Roguelike", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);

	// Make sure creating the window succeeded
	if (!window_g) {
		system("pause");
		return 1;
	}

	//Create renderer for window
	renderer_g = SDL_CreateRenderer(window_g, -1, SDL_RENDERER_ACCELERATED);
	if (renderer_g == NULL)
	{
		return 1;
	}

	//Initialize renderer color
	SDL_SetRenderDrawColor(renderer_g, 0, 0, 0, 0);

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

	// Get the surface from the window
	winSurface_g = SDL_GetWindowSurface(window_g);

	// Make sure getting the surface succeeded
	if (!winSurface_g) {
		system("pause");
		return 1;
	}

	// set up viewports
	mapView_g.x = 0;
	mapView_g.y = 0;
	mapView_g.w = SCREEN_WIDTH / 2;
	mapView_g.h = SCREEN_HEIGHT / 2;
	statsView_g.x = SCREEN_WIDTH / 2;;
	statsView_g.y = 0;
	statsView_g.w = SCREEN_WIDTH / 2;
	statsView_g.h = SCREEN_HEIGHT / 2;
	infoView_g.x = 0;
	infoView_g.y = SCREEN_HEIGHT / 2;
	infoView_g.w = SCREEN_WIDTH / 2;
	infoView_g.h = SCREEN_HEIGHT / 2;

	// set up the text color
	textColor_g.r = 255;
	textColor_g.g = 255;
	textColor_g.b = 255;

	// Fill the window with a black rectangle
	SDL_FillRect(winSurface_g, NULL, SDL_MapRGB(winSurface_g->format, 0, 0, 0));

	// Update the window display
	SDL_UpdateWindowSurface(window_g);
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
	while (!fonts_g.empty()) {
		delete fonts_g.back();
	}
	fonts_g.pop_back();
	SDL_FreeSurface(winSurface_g);
	winSurface_g = NULL;
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
			}
		}
	}
	return -1;
}

void loadFonts() {
	TTF_Font* font = TTF_OpenFont("Dawnlike Tiles/GUI/SDS_8x8.ttf", 12);
	fonts_g.push_back(font);
}
