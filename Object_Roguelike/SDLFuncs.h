#pragma once
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <string>
#include <vector>

#define SCREEN_WIDTH 1600
#define SCREEN_HEIGHT 768
#define TILE_SOURCE_SIZE 16
#define MAX_MAP_SIZE 21
#define FRAME_WAIT 500

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
	EVENT_KEY_ESC,
	EVENT_RESIZE
};

enum dirEnums {
	UP,
	DOWN,
	LEFT,
	RIGHT
};

#define NUM_FONTS 4
enum fontEnums {
	FONT_8,
	FONT_16,
	FONT_24,
	FONT_32,
};

enum viewEnums {
	MAP,
	STATS,
	INFO
};

#define NUM_STAT_LINES 14
enum statEnums {
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
extern int tileSize_g;
extern int currentFrame_g;
extern int drawFrame_g;

int SDL_Init();
void SDL_Close();
bool loadImage(SDL_Texture* &image, std::string path);
int handleEvents();
void loadFonts();
void freeFonts();
void rendererInit();
int getTextSpace();
void frameClock();
int inputEventFilter(void* data, SDL_Event* event);
int noFilter(void* data, SDL_Event* event);
void filterInputEvents();
void resetFilter();
void clearEvents();
