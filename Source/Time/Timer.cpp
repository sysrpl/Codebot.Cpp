#include <Codebot/Time/Timer.h>
#include <Codebot/Time/PerformanceTime.h>

namespace Codebot
{
namespace Time
{

Timer::Timer() :
	frequency(PerformanceFrequency()), origin(PerformanceCounter()), current(origin)
{
}

void Timer::Update()
{
	auto value = PerformanceCounter();
	elapsed = (value - current) / frequency;
	time = (value - origin) / frequency;
	current = value;
	frame++;
}

Double Timer::Elapsed() const
{
	return elapsed;
}

Double Timer::Time() const
{
	return time;
}

Integer Timer::Fps() const
{
	static const Double epsilon = 0.0001;
	if (elapsed < epsilon)
		return 999;
	if (elapsed > 1)
		return 0;
	return 1.0 / elapsed;
}

LargeWord Timer::Frame() const
{
	return frame;
}

}
}
