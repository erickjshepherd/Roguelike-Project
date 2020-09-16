#include "Floor.h"

Floor::Floor() {
	blocking = 0;
	icon = '.';
	name = "floor";
	description = "";

	// sprite
	spritePath = TILEPATH;
	spriteSheetW = 8;
	spriteType = 19;
}

Floor::~Floor() {

}
