#include "game.hpp"
#include "../engine/src/core/logger.hpp"


b8 game_initialize(game * game_inst) {
	
	WDEBUG(L"game_initialize() called");
	
	return true;
}

b8 game_update(game * game_inst, f32 dT) {
	return true;
}

b8 game_render(game * game_inst, f32 dT) {
	return true;
}

b8 game_on_resize(game * game_inst, u32 width, u32 height) {
	return true;
}