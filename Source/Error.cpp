#include <Error.h>


#include <Platforms.h>

#define FMT_HEADER_ONLY  // TODO: Move this
#include <fmt/core.h>

#if (PLATFORM_IS_WIN32 || PLATFORM_IS_WIN64)
#include <windows.h>
#endif

#include <iostream>


void PrintError(const char* msg)
{
#if (PLATFORM_IS_WIN32 || PLATFORM_IS_WIN64)
	::OutputDebugStringA(msg);
	::OutputDebugStringA("\n");
#endif
	std::cout << msg << "\n";
}

void FatalError(const char* msg, int line, const char* func)
{
	PrintError(fmt::format("Fatal Error in {}({}): {}", func, line, msg).c_str());
	PrintError("Fatal Error in");
	std::exit(-1);
}
