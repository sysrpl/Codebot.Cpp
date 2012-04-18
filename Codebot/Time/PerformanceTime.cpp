#include <Codebot/Time/PerformanceTime.h>
#ifdef WindowsOS
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#else
#include <sys/time.h>
#endif

namespace Codebot
{
namespace Time
{

#ifdef WindowsOS
#else
static const LargeWord usec_per_sec = 1000000;

LargeWord PerformanceFrequency()
{
	return usec_per_sec;
}

LargeWord PerformanceCounter()
{
    struct timeval time;
    gettimeofday(&time, null);
    return (LargeWord)time.tv_usec + (LargeWord)time.tv_sec * usec_per_sec;
}
#endif

}
}
