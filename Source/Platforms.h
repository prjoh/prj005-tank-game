#pragma once


#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
#ifdef _WIN64
#define PLATFORM_IS_WIN64 1
#else
#define PLATFORM_IS_WIN32 1
#endif

#elif __APPLE__
#include <TargetConditionals.h>
#if TARGET_IPHONE_SIMULATOR
#define PLATFORM_IS_IOS_SIM 1
#elif TARGET_OS_MACCATALYST
// Mac's Catalyst (ports iOS API into Mac, like UIKit).
#elif TARGET_OS_IPHONE
#define PLATFORM_IS_IOS 1
#elif TARGET_OS_MAC
#define PLATFORM_IS_OSX 1
#else
#   error "Unknown Apple platform"
#endif

#elif __linux__
// TODO

#elif __ANDROID__
// TODO

#endif
