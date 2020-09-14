#include "Floor.h"

Floor::Floor() {
	blocking = 0;
	icon = '.';
	name = "floor";
	description = "It's the floor";

	// sprite
	spritePath = TILEPATH;
	spriteSheetW = 8;
	spriteType = 19;
}

Floor::~Floor() {

}
