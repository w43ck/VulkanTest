#pragma once

#include "core/application.hpp"
#include "../../src/game.hpp"

#include <functional>
#include <memory>

// basic game state
struct game {
	app_config app_conf;
	
	// b8 (*initialize)(struct game & game_inst);
	using Initialize = std::function<b8(game *)>;
	Initialize initialize = nullptr;
	
	// b8 (*update)(struct game & game_inst, f32 dT);
	using Update = std::function<b8(game *, f32)>;
	Update update;
	
	using Render = std::function<b8(game *, f32)>;
	Render render;
	
	using On_resize = std::function<void(game *, u32, u32)>;
	On_resize on_resize;
	
	std::unique_ptr<game_state> state;
	
};