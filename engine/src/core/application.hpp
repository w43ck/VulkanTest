#pragma once

#include "../defines.hpp"

#include <string>

struct game;

// app config
struct app_config {
	i16 start_pos_x;
	i16 start_pos_y;
	i16 start_width;
	i16 start_height;
	std::wstring name;
};

WAPI b8 app_create(game & game_inst);

WAPI b8 app_run();