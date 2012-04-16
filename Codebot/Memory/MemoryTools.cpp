#include "Codebot/Memory/MemoryTools.h"

namespace Codebot
{
namespace Memory
{

#pragma message(Reminder "test MemCompare()")
Boolean MemCompare(Pointer a, Pointer b, Cardinal size)
{
	if (size == 0)
		return true;
	return std::equal((Byte*)a, (Byte*)a + size, (Byte*)b);
}

#pragma message(Reminder "test MemCopy()")
void MemCopy(Pointer source, Pointer dest, Cardinal size)
{
	if (size == 0)
		return;
	std::copy((Byte*)source, (Byte*)source + size, (Byte*)dest);
}

#pragma message(Reminder "test MemZero()")
void MemZero(Pointer p, Cardinal size)
{
	if (size == 0)
		return;
	std::fill_n((Byte*)p, size, 0); 
}

}
}
