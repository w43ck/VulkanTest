#include "engine/src/core/logger.hpp"
#include "engine/src/core/asserts.hpp"

// #include "engine/src/platform/platform.hpp"

int main() {
	
	WFATAL(L"A test message: {}", 3.14f);
	WERROR(L"A test message: {}", 3.14f);
	WWARN(L"A test message: {}", 3.14f);
	WINFO(L"A test message: {}", 3.14f);
	WDEBUG(L"A test message: {}", 3.14f);
	WTRACE(L"A test message: {}", 3.14f);
	
	platform_state state;
	if(platform_startup(&state, L"App", 100, 100, 1280, 720)) {
		while(1) {
			platform_pump_messages(&state);
		}
	}
	
	platform_shutdown(&state);
	
	return 0;
}