#pragma once
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <string>
#include <vector>

#define SCREEN_WIDTH 1280
#define SCREEN_HEIGHT 720
#define TILE_SIZE 16

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

enum viewEnums {
	MAP,
	STATS,
	INFO
};

#define TEXTSPACE 16
enum statEnums {
		BLANK1,
		HEALTH,
		STRENGTH,
		FLOOR,
		ROOMS,
		WEAPON,
		HEAD,
		CHEST,
		LEGS,
		SPELL1,
		SPELL2,
		SPELL3,
		BLANK2,
		INSPECT,
		INSPECTINFO
};

extern SDL_Surface* winSurface_g;
extern SDL_Window* window_g;
extern SDL_Renderer* renderer_g;
extern std::vector<TTF_Font*> fonts_g;
extern SDL_Rect mapView_g;
extern SDL_Rect statsView_g;
extern SDL_Rect eventsView_g;
extern SDL_Color textColor_g;

int SDL_Init();
void SDL_Close();
bool loadImage(SDL_Texture* &image, std::string path);
int handleEvents();
void loadFonts();
