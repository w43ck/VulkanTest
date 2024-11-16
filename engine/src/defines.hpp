#pragma once

// UINT types
using u8 = unsigned char;
using u16 = unsigned short;
using u32 = unsigned int;
using u64 = unsigned long long;

// SINT types
using i8 = signed char;
using i16 = signed short;
using i32 = signed int;
using i64 = signed long long;

// Floating point types
using f32 = float;
using f64 = double;

// Boolean types
using b32 = int;
using b8 = char;

// Properly defines static assertions
#if defined(__clang__) || defined(__gcc__)
#define STATIC_ASSERT _Static_assert
#else
#define STATIC_ASSERT static_assert
#endif

// Ensure all types are of the correct size
STATIC_ASSERT(sizeof(u8)==1, "Expected u8 to be 1 byte");
STATIC_ASSERT(sizeof(u16)==2, "Expected u8 to be 2 byte");
STATIC_ASSERT(sizeof(u32)==4, "Expected u8 to be 4 byte");
STATIC_ASSERT(sizeof(u64)==8, "Expected u8 to be 8 byte");

STATIC_ASSERT(sizeof(i8)==1, "Expected i8 to be 1 byte");
STATIC_ASSERT(sizeof(i16)==2, "Expected i8 to be 2 byte");
STATIC_ASSERT(sizeof(i32)==4, "Expected i8 to be 4 byte");
STATIC_ASSERT(sizeof(i64)==8, "Expected i8 to be 8 byte");

STATIC_ASSERT(sizeof(f32)==4, "Expected f32 to be 4 byte");
STATIC_ASSERT(sizeof(f64)==8, "Expected f64 to be 8 byte");

#define TRUE 1
#define FALSE 0

// Platform detection
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__)
#define WPLATFORM_WINDOWS 1
#ifndef _WIN64
#error "64-bit is required on Windows!"
#endif
#elif defined(__linux__) || defined(__gnu_linux__)
// Linux OS
#define WPLATFORM_LINUX 1
#if defined(__ANDROID__)
#define WPLATFORM_ANDROID 1
#endif
#elif defined(__unix__)
#define WPLATFORM_UNIX 1
#elif defined(_POSIX_VERSION)
#define WPLATFORM_POSIX 1
#elif __APPLE__
#define WPLATFORM_APPLE 1
#include <TargetConditionals.h>
#if TARGET_IPHONE_SIMULATOR
#define WPLATFORM_IOS 1
#define WPLATFORM_IOS_SIMULATOR 1
#elif TARGET_OS_IPHONE
#define WPLATFORM_IOS 1
#elif TARGET_OS_MAC
// 
#else
#error "Unknown Apple Platform!"
#endif
#else
#error "Unknown Platform!"
#endif

#ifdef WEXPORT
// Exports
#ifdef _MSC_VER
#define WAPI __declspec(dllexport)
#else
#define WAPI __attribute__((visibility("default")))
#endif
#else
// Imports
#ifdef _MSC_VER
#define WAPI __declspec(dllimport)
#else
#define WAPI
#endif
#endif
