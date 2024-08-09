#include <System.h>


#if (DEBUG_BUILD)
#include <Game.h>

void pd::System::debugUpdate(f32 timeElapsedS)
{
	m_systemStart = std::chrono::high_resolution_clock::now();
	
	update(timeElapsedS);
	
	auto endTime = std::chrono::high_resolution_clock::now();
	auto time = std::chrono::duration_cast<std::chrono::microseconds>(endTime - m_systemStart).count();
	
	m_timeMilli = time / 1000;
	m_timeMicro = time % 1000;

	Game::s_SystemTimes.emplace_back(getTimeString());
}

std::string pd::System::getTimeString()
{
	return m_name + ": " + std::to_string(m_timeMilli) + "." + std::to_string(m_timeMicro / 100) + "ms";
}
#endif // DEBUG_BUILD