// #include "platform.hpp"
#include "../core/logger.hpp"

// Win platform layer
#if WPLATFORM_WINDOWS


#include <Windows.h>
#include <Windowsx.h> // param input extraction
#include <stdlib.h>


struct internal_state {
	HINSTANCE hInst;
	HWND hWnd;
};

static f64 clock_frequency;
static LARGE_INTEGER start_time;

LRESULT CALLBACK win_process_message(HWND hWnd, u32 msg, WPARAM wP, LPARAM lP);

b8 platform_startup(platform_state * pf_state, std::wstring_view app_name, i32 x, i32 y, i32 width, i32 height) {
	pf_state->internal_state = malloc(sizeof(internal_state));
	internal_state * state = static_cast<internal_state *>(pf_state->internal_state);
	state->hInst = GetModuleHandleW(nullptr);
	
	// Setup and register wnd class
	HICON icon = LoadIcon(state->hInst, IDI_APPLICATION);
	WNDCLASSW wc;
	std::memset(&wc, 0, sizeof(wc));
	wc.style = CS_DBLCLKS; // Get double-clicks
	wc.lpfnWndProc = win_process_message;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = state->hInst;
	wc.hIcon = icon;
	wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wc.hbrBackground = nullptr; // transparent
	wc.lpszClassName = L"wWindowClass";
	
	if(!RegisterClassW(&wc)) {
		MessageBoxW(nullptr, L"Window registration failed!", L"Error", MB_ICONEXCLAMATION | MB_OK);
		return false;
	}
	
	// create wnd
	u32 client_x = x;
	u32 client_y = y;
	u32 client_w = width;
	u32 client_h = height;
	
	u32 wnd_x = client_x;
	u32 wnd_y = client_y;
	u32 wnd_w = client_w;
	u32 wnd_h = client_h;
	
	u32 wnd_style = WS_OVERLAPPED | WS_SYSMENU | WS_CAPTION;
	u32 wnd_ex_style = WS_EX_APPWINDOW;
	
	wnd_style |= WS_MAXIMIZEBOX;
	wnd_style |= WS_MINIMIZEBOX;
	wnd_style |= WS_THICKFRAME;
	
	RECT border_rect = {0, 0, 0, 0};
	AdjustWindowRectEx(&border_rect, wnd_style, 0, wnd_ex_style);
	
	wnd_x += border_rect.left;
	wnd_y += border_rect.top;
	
	wnd_w += border_rect.right - border_rect.left;
	wnd_h += border_rect.bottom - border_rect.top;
	
	HWND handle = CreateWindowExW(wnd_ex_style, L"wWindowClass", app_name.data(), wnd_style, wnd_x, wnd_y, wnd_w, wnd_h, nullptr, nullptr, state->hInst, nullptr);
	
	if(handle == nullptr) {
		MessageBoxW(nullptr, L"Window creation failed!", L"Error", MB_ICONEXCLAMATION | MB_OK);
		WFATAL(L"Window creation failed!");
		return false;
	} else {
		state->hWnd = handle;
	}
	
	// show the wnd
	b32 should_activate = 1; // for input
	i32 show_wnd_command_flags = should_activate ? SW_SHOW : SW_SHOWNOACTIVATE;
	// if initially minimized, use SW_MINIMIZE : SW_SHOWMINNOACTIVE;
	// if initially maximized, use SW_SHOWMAXIMIZED : SW_MAXIMIZE;
	ShowWindow(state->hWnd, show_wnd_command_flags);
	
	LARGE_INTEGER frequency;
	QueryPerformanceFrequency(&frequency);
	clock_frequency = 1.f / static_cast<f64>(frequency.QuadPart);
	QueryPerformanceCounter(&start_time);
	
	return true;
	
}

void platform_shutdown(platform_state * pf_state) {
	internal_state * state = static_cast<internal_state *>(pf_state->internal_state);
	
	if(state->hWnd) {
		DestroyWindow(state->hWnd);
		state->hWnd = nullptr;
	}
}

b8 platform_pump_messages(platform_state * pf_state) {
	MSG msg;
	while(PeekMessageW(&msg, nullptr, 0, 0, PM_REMOVE)) {
		TranslateMessage(&msg);
		DispatchMessageW(&msg);
	}
	
	return true;
}

void * platfrom_allocate(const u64 size, const b8 aligned) {
	return malloc(size);
}

void platform_free(void * block, const b8 aligned) {
	free(block);
}

void * platform_zero_memory(void * block, const u64 size) {
	return memset(block, 0, size);
}

void * platform_copy_memory(void * dest, const void * source, const u64 size) {
	return memcpy(dest, source, size);
}

void * platform_set_memory(void * dest, const i32 value, const u64 size) {
	return memset(dest, value, size);
}

void platform_console_write(std::wstring_view mes, u8 clr) {
	HANDLE console_handle = GetStdHandle(STD_OUTPUT_HANDLE);
	// FATAL, ERROR, WARN, INFO, DEBUG, TRACE
	static u8 levels[6] = {64, 4, 6, 2, 1, 8};
	SetConsoleTextAttribute(console_handle, levels[clr]);
	
	OutputDebugStringW(mes.data());
	LPDWORD number_written = 0;
	std::wstring str(mes.data(), mes.size());
	str += L"\n";
	WriteConsoleW(console_handle, str.data(), static_cast<DWORD>(str.length()), number_written, nullptr);
}

void platform_console_write_error(std::wstring_view mes, u8 clr) {
	HANDLE console_handle = GetStdHandle(STD_ERROR_HANDLE);
	// FATAL, ERROR, WARN, INFO, DEBUG, TRACE
	static u8 levels[6] = {64, 4, 6, 2, 1, 8};
	SetConsoleTextAttribute(console_handle, levels[clr]);
	
	OutputDebugStringW(mes.data());
	LPDWORD number_written = 0;
	std::wstring str(mes.data(), mes.size());
	str += L"\n";
	WriteConsoleW(console_handle, str.data(), static_cast<DWORD>(str.length()), number_written, nullptr);
}

f64 platform_get_abs_time() {
	LARGE_INTEGER now_time;
	QueryPerformanceCounter(&now_time);
	return static_cast<f64>(now_time.QuadPart * clock_frequency);
}

void platform_sleep(u64 ms) {
	Sleep(ms);
}

LRESULT CALLBACK win_process_message(HWND hWnd, u32 msg, WPARAM wP, LPARAM lP) {
	switch(msg) {
		case WM_ERASEBKGND: {
			return 1;
		}
		case WM_CLOSE: {
			// TODO: Fire an event for the app to quit
			
			return 0;
		}
		case WM_DESTROY: {
			PostQuitMessage(0);
			return 0;
		}
		case WM_SIZE: {
			RECT r;
			GetClientRect(hWnd, &r);
			u32 width = r.right - r.left;
			u32 height = r.bottom - r.top;
		} break;
		case WM_KEYDOWN:
		case WM_SYSKEYDOWN:
		case WM_KEYUP:
		case WM_SYSKEYUP: {
			b8 pressed = (msg == WM_KEYDOWN || msg == WM_SYSKEYUP);
		} break;
		case WM_MOUSEMOVE: {
			i32 m_x_pos = GET_X_LPARAM(lP);
			i32 m_y_pos = GET_Y_LPARAM(lP);
		} break;
		case WM_MOUSEWHEEL: {
			i32 dZ = GET_WHEEL_DELTA_WPARAM(wP);
			if(dZ != 0) {
				dZ = dZ < 0 ? -1 : 1;
			}
		} break;
		case WM_LBUTTONDOWN:
		case WM_MBUTTONDOWN:
		case WM_RBUTTONDOWN:
		case WM_LBUTTONUP:
		case WM_MBUTTONUP:
		case WM_RBUTTONUP: {
			b8 pressed = msg == WM_LBUTTONDOWN || msg == WM_MBUTTONDOWN || msg == WM_RBUTTONDOWN;
		} break;
		
	}
	
	return DefWindowProcW(hWnd, msg, wP, lP);
}


#endif // WPLATFORM_LAYER