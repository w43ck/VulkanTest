#include "../../src/game.hpp"
#include "core/logger.hpp"

// #include "../engine/src/platform/platform.hpp"
#include "../src/game_types.hpp"

b8 create_game(game & out_game) {
	
	out_game.app_conf.start_pos_y = 100;
	out_game.app_conf.start_width = 1280;
	out_game.app_conf.start_height = 720;
	out_game.app_conf.start_pos_x = 100;
	out_game.app_conf.name = L"App";
	
	out_game.initialize = game_initialize;
	out_game.render = game_render;
	out_game.update = game_update;
	out_game.on_resize = game_on_resize;
	
	out_game.state = std::make_unique<game_state>();
	
	return true;
}

/*
	The main entry point of the app
*/
int main() {
	
	// Request the game instance from the app
	game game_inst;
	
	if(!create_game(game_inst)) {
		WFATAL(L"Couldn't create a game!");
		return -1;
	}
	
	// Ensure the function pointers exist
	if(!game_inst.render || !game_inst.update || !game_inst.initialize || !game_inst.on_resize) {
		WFATAL(L"The game's func ptrs must be assigned!");
		return -2;
	}
	
	if(!app_create(game_inst)) {
		WINFO(L"App failed to create");
		return 1;
	}
	
	if(!app_run()) {
		WINFO(L"App did not shutdown gracefully");
		return 2;
	}
	
	return 0;
}