#include "application.hpp"
#include "logger.hpp"
#include "../platform/platform.hpp"
#include "../game_types.hpp"
#include "wmemory.hpp"

struct app_state {
	
	std::unique_ptr<game> game_inst;
	
	b8 is_running;
	b8 is_suspended;
	platform_state pf_state;
	i16 width;
	i16 height;
	f64 last_time;
};

static b8 initialized = false;
static app_state app_st;

b8 app_create(game & game_inst) {
	if(initialized) {
		WERROR(L"app_create called more than once");
		return false;
	}
	
	app_st.game_inst = std::make_unique<game>(std::move(game_inst));
	
	// initialize subsystems
	init_logging();
	
	WFATAL(L"A test message: {}", 3.14f);
	WERROR(L"A test message: {}", 3.14f);
	WWARN(L"A test message: {}", 3.14f);
	WINFO(L"A test message: {}", 3.14f);
	WDEBUG(L"A test message: {}", 3.14f);
	WTRACE(L"A test message: {}", 3.14f);
	
	app_st.is_running = true;
	app_st.is_suspended = false;
	
	if(!platform_startup(&app_st.pf_state,
			app_st.game_inst->app_conf.name,
			app_st.game_inst->app_conf.start_pos_x,
			app_st.game_inst->app_conf.start_pos_y,
			app_st.game_inst->app_conf.start_width,
			app_st.game_inst->app_conf.start_height)) {
		
		return false;
	}
	
	// Initialize the game
	if(!app_st.game_inst->initialize(app_st.game_inst.get())) {
		WFATAL(L"Game failed to initialize");
		return false;
	}
	
	app_st.game_inst->on_resize(app_st.game_inst.get(), app_st.width, app_st.height);
	
	initialized = true;
	
	return true;
	
}

b8 app_run() {
	
	WINFO(get_memory_usage_str());
	
	while(app_st.is_running) {
			if(!platform_pump_messages(&app_st.pf_state)) {
				app_st.is_running = false;
			}
			
			if(!app_st.is_suspended) {
				if(!app_st.game_inst->update(app_st.game_inst.get(), 0)) {
					WFATAL(L"Game updating failed, shutting down");
					app_st.is_running = false;
					break;
				}
				
				if(!app_st.game_inst->render(app_st.game_inst.get(), 0)) {
					WFATAL(L"Game render failed, shutting down");
					app_st.is_running = false;
					break;
				}
			}
	}
	
	app_st.is_running = false;
	
	platform_shutdown(&app_st.pf_state);
	
	return true;
}