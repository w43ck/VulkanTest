#include "engine/src/core/logger.hpp"
#include "engine/src/core/asserts.hpp"

int main() {
	
	WFATAL("A test message: {}", 3.14f);
	WERROR("A test message: {}", 3.14f);
	WWARN("A test message: {}", 3.14f);
	WINFO("A test message: {}", 3.14f);
	WDEBUG("A test message: {}", 3.14f);
	WTRACE("A test message: {}", 3.14f);
	
	WASSERT(1 == 0);
	
	return 0;
}