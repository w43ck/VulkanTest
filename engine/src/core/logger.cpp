#include "logger.hpp"
#include "asserts.hpp"

// TODO: temporary
// ...

void report_assertion_failure(const char * expression, const char * message, const char * file, i32 line) {
	log_output(log_lvl::LOG_LEVEL_FATAL, "Assertion Failure: {}, message: {}, in file: {}, line: {}\n", expression, message, file, line);
}


b8 init_logging() {
	// TODO: create log file
	
	return 1;
}

void shutdown_logging() {
	// TODO: cleanup logging/write queued entries
	
	
}
