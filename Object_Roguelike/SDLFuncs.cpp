#include "SDLFuncs.h"

SDL_Surface* winSurface_g;
SDL_Window* window_g;
SDL_Renderer* renderer_g;

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
	SDL_SetRenderDrawColor(renderer_g, 0xFF, 0xFF, 0xFF, 0xFF);

	//Initialize PNG loading
	int imgFlags = IMG_INIT_PNG;
	if (!(IMG_Init(imgFlags) & imgFlags))
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

	// Fill the window with a white rectangle
	SDL_FillRect(winSurface_g, NULL, SDL_MapRGB(winSurface_g->format, 255, 255, 255));

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
	SDL_FreeSurface(winSurface_g);
	winSurface_g = NULL;
	SDL_DestroyRenderer(renderer_g);
	renderer_g = NULL;
	SDL_DestroyWindow(window_g);
	window_g = NULL;

	IMG_Quit();
	SDL_Quit();
}

void handleEvents() {
	/* Poll for events */
	SDL_Event event;
	int quit = 0;
	while (SDL_PollEvent(&event)) {

		switch (event.type) {
			/* Keyboard event */
			/* Pass the event data onto PrintKeyInfo() */
		case SDL_KEYDOWN:
		case SDL_KEYUP:
			break;

			/* SDL_QUIT event (window close) */
		case SDL_QUIT:
			quit = 1;
			break;

		default:
			break;
		}

	}
}
