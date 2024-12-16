#pragma once

#include "../engine/src/defines.hpp"

struct game;

struct game_state {
	
	f32 dT;
	
};

b8 game_initialize(game * game_inst);

b8 game_update(game * game_inst, f32 dT);

b8 game_render(game * game_inst, f32 dT);

b8 game_on_resize(game * game_inst, u32 width, u32 height);