#pragma once


#include <string>
#include <array>

#include <Types.h>


namespace pd
{
	class Guid
	{
		friend struct std::hash<pd::Guid>;
	public:
		constexpr Guid() : Guid(0, 0, 0, 0) {};
		constexpr Guid(u32 v0, u32 v1, u32 v2, u32 v3) : m_v{ v0, v1, v2, v3 } {};

		static Guid Generate();

		std::string toString() const;

		constexpr bool isEmpty() const { return *this == Guid{}; }

		constexpr u32 get0() const { return m_v[0]; }
		constexpr u32 get1() const { return m_v[1]; }
		constexpr u32 get2() const { return m_v[2]; }
		constexpr u32 get3() const { return m_v[3]; }

		constexpr bool operator<(Guid o) const { return GuidCompare(m_v, o.m_v) < 0; }
		constexpr bool operator<=(Guid o) const { return GuidCompare(m_v, o.m_v) <= 0; }
		constexpr bool operator>(Guid o) const { return GuidCompare(m_v, o.m_v) > 0; }
		constexpr bool operator>=(Guid o) const { return GuidCompare(m_v, o.m_v) >= 0; }
		constexpr bool operator==(Guid o) const { return GuidCompare(m_v, o.m_v) == 0; }
		constexpr bool operator!=(Guid o) const { return GuidCompare(m_v, o.m_v) != 0; }

	private:
		constexpr s32 GuidCompare(const std::array<u32, 4>& a, const std::array<u32, 4>& b) const
		{
			for (s32 i = 0; i < 4; ++i)
			{
				if (a[i] == b[i]) continue;
				if (a[i] > b[i]) return 1;
				if (a[i] < b[i]) return -1;
			}
			return 0;
		}

		std::array<u32, 4> m_v;
	};
}


template<>
struct std::hash< pd::Guid >
{
	[[nodiscard]] size_t operator()(pd::Guid guid) const noexcept
	{
		size_t seed = 0;
		for (auto v : guid.m_v)
		{
			seed ^= std::hash<u32>{}(v)+0x9e3779b9 + (seed << 6) + (seed >> 2);
		}
		return seed;
	}
};
