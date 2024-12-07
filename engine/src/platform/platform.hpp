#pragma once

#include "../defines.hpp"

#include <string>

struct platform_state {
	void * internal_state;
};

WAPI b8 platform_startup(platform_state * pf_state, std::wstring_view app_name, i32 x, i32 y, i32 width, i32 height);

WAPI void platform_shutdown(platform_state * pf_state);

WAPI b8 platform_pump_messages(platform_state * pf_state);


void * platfrom_allocate(const u64 size, const b8 aligned);
void platform_free(void * block, const b8 aligned);
void * platform_zero_memory(void * block, const u64 size);
void * platform_copy_memory(void * dest, const void * source, const u64 size);
void * platform_set_memory(void * dest, const i32 value, const u64 size);

WAPI void platform_console_write(std::wstring_view mes, u8 clr);
WAPI void platform_console_write_error(std::wstring_view mes, u8 clr);

f64 platform_get_abs_time();

// Sleep on the thread for the provided ms. This blocks the main thread.
// Should only be used for giving time back to the OS for unused update power.
// Therefore it is not exported.
void platform_sleep(u64 ms);
