#pragma once


#include <Config.h>


extern "C" __declspec(dllimport) void __stdcall DebugBreak(void);
extern "C" __declspec(dllimport) int __stdcall IsDebuggerPresent(void);
#define BREAKPOINT() do { if(::IsDebuggerPresent()) { ::DebugBreak(); } } while(false)


void PrintError(const char* msg);
void FatalError(const char* msg, int line, const char* func);

#define ABORT_INTERNAL(message) FatalError(message, __LINE__, __func__);

#define ABORT(message, ...) \
	do {										\
		if (!(condition))						\
		{										\
			BREAKPOINT();						\
			/* TODO: AbortMsgHandlerWithArgs */ \
			std::exit(-1);						\
		}										\
	} while (false)


#define ASSERT_INTERNAL(condition, message)			  \
	do {											  \
		if (!(condition))							  \
		{											  \
			BREAKPOINT();							  \
			FatalError(message, __LINE__, __func__);  \
		}											  \
	} while (false)

#if ASSERT_ENABLED

#define ASSERT_MSG(condition, message, ...)		\
	do {										\
		if (!(condition))						\
		{										\
			BREAKPOINT();						\
			/* TODO: AssertMsgHandlerWithArgs */\
			std::exit(-1);						\
		}										\
	} while (false)

#define ASSERT(condition) ASSERT_MSG(condition, #condition)

#else

#define ASSERT_MSG(...) (void)(0)
#define ASSERT(...) (void)(0)

#endif
