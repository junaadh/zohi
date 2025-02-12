#pragma once

// unsigned int types
typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;
typedef unsigned long long u64;

// signed int types
typedef signed char i8;
typedef signed short i16;
typedef signed int i32;
typedef signed long long i64;

// floating point types
typedef float f32;
typedef double f64;

// boolean types
typedef int b32;
typedef char b8;

// define static assertions
#if defined(__clang__) || defined(__gcc__)
#define STATIC_ASSERT _Static_assert
#else
#define STATIC_ASSERT static_assert
#endif

// assert that all types are of correct size
STATIC_ASSERT(sizeof(u8) == 1, "Expected u8 to be 1 byte.");
STATIC_ASSERT(sizeof(u16) == 2, "Expected u16 to be 2 bytes.");
STATIC_ASSERT(sizeof(u32) == 4, "Expected u16 to be 4 bytes.");
STATIC_ASSERT(sizeof(u64) == 8, "Expected u16 to be 8 bytes.");

STATIC_ASSERT(sizeof(i8) == 1, "Expected i8 to be 1 byte.");
STATIC_ASSERT(sizeof(i16) == 2, "Expected i16 to be 2 bytes.");
STATIC_ASSERT(sizeof(i32) == 4, "Expected i16 to be 4 bytes.");
STATIC_ASSERT(sizeof(i64) == 8, "Expected i16 to be 8 bytes.");

STATIC_ASSERT(sizeof(f32) == 4, "Expected f32 to be 4 byte.");
STATIC_ASSERT(sizeof(f64) == 8, "Expected f64 to be 8 bytes.");

// true false
#define TRUE 1
#define FALSE 0

// platform detection
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__)
#define ZPLATFORM_WINDOWS 1
#ifndef _WIN64
#error "64-bit is required on Windows"
#endif
#elif defined(__linux__) || defined(__gnu_linux__)
// linux specific
#define ZPLATFORM_LINUX 1
#if defined(__ANDROID__)
#define ZPLATFORM_ANDROID 1
#endif
#elif defined(__unix__)
// catch others
#define ZPLATFORM_UNIX 1
#elif defined(_POSIX_VERSION)
// posix
#define ZPLATFORM_POSIX 1
#elif __APPLE__
// apple platforms
#define ZPLATFORN_APPLE 1
#include <TargetConditionals.h>
#if TARGET_IPHONE_SIMULATOR
// ios sim
#define ZPLATFORM_IOS 1
#define ZPLATFORM_IOS_SIMULATOR 1
#elif TARGET_OS_IPHONE
// ios
#define ZPLATFORM_IOS 1
#elif TARGET_OS_MAC
// mac os
#else
#error "unknown apple platform"
#endif
#else
#error "unknown platform
#endif

#ifdef ZEXPORT
// exports
#ifdef _MSC_VER
#define ZAPI __declspec(dllexport)
#else
#define ZAPI __attribute__((visibility("default")))
#endif
#else
// imports
#ifdef _MSC_VER
#define ZAPI __declspec(dllimport)
#else
#define ZAPI
#endif
#endif
