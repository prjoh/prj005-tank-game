#include <Guid.h>


#include <Error.h>
#include <Platforms.h>


#if (PLATFORM_IS_WIN32 || PLATFORM_IS_WIN64)
#include <combaseapi.h>
#else
// TODO
//Linux (using libuuid),
//MacOS (using CFUUID),
//iOS (also using CFUUID),
//Android (using JNI calls to java.util.UUID).
#endif


using namespace pd;


Guid Guid::Generate()
{
	Guid guid;
	auto res = CoCreateGuid(reinterpret_cast<GUID*>(&guid));
	ASSERT_MSG(res == S_OK, "Failed to create Guid: CoCreateGuidi failed.");
	return guid;
}

std::string Guid::toString() const
{
	char str[37];
	sprintf_s(str, "%08x-%08x-%08x-%08x", m_v[0], m_v[1], m_v[2], m_v[3]);
	return str;
}
