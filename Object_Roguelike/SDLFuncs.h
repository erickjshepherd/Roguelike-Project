#pragma once
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <string>
#include <vector>

#define SCREEN_WIDTH 1280
#define SCREEN_HEIGHT 720

enum eventEnums {
	EVENT_QUIT,
	EVENT_KEY_UP,
	EVENT_KEY_DOWN,
	EVENT_KEY_LEFT,
	EVENT_KEY_RIGHT,
	EVENT_KEY_1,
	EVENT_KEY_2,
	EVENT_KEY_3,
	EVENT_KEY_ENTER,
	EVENT_KEY_ESC
};

enum fontEnums {
	FONT_12
};

extern SDL_Surface* winSurface_g;
extern SDL_Window* window_g;
extern SDL_Renderer* renderer_g;
extern std::vector<TTF_Font*> fonts_g;

int SDL_Init();
void SDL_Close();
bool loadImage(SDL_Texture* &image, std::string path);
int handleEvents();
void loadFonts();
