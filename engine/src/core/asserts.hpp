#pragma once

#include "../defines.hpp"

#define WASSERTIONS_ENABLED

#ifdef WASSERTIONS_ENABLED
#if _MSC_VER
#include <intrin.h>
#define debugBreak() __debugbreak()
#else
#define debugBreak() __builtin_trap()
#endif

WAPI void report_assertion_failure(const char * expression, const char * message, const char * file, i32 line);

#define WASSERT(expr) {\
	if(expr){\
	}else{\
		report_assertion_failure(#expr, "", __FILE__, __LINE__);\
		debugBreak();\
	}\
}

#define WASSERT_MSG(expr, msg) {\
	if(expr){\
	}else{\
		report_assertion_failure(#expr, msg, __FILE__, __LINE__);\
		debugBreak();\
	}\
}

#ifdef _DEBUG
#define WASSERT_DEBUG(expr) {\
	if(expr){\
	}else{\
		report_assertion_failure(#expr, "", __FILE__, __LINE__);\
		debugBreak();\
	}\
}
#else
#define WASSERT_DEBUG(expr)
#endif

#else

#define WASSERT(expr)
#define WASSERT_MSG(expr, msg)
#define WASSERT_DEBUG(expr)
#endif
