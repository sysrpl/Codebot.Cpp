#include "Codebot/Memory/MemoryTools.h"

namespace Codebot
{
namespace Memory
{

// TODO: Write tests for memory functions

Boolean MemCompare(Pointer a, Pointer b, Cardinal size)
{
	if (size == 0)
		return true;
	return std::equal((Byte*)a, (Byte*)a + size, (Byte*)b);
}

void MemCopy(Pointer source, Pointer dest, Cardinal size)
{
	if (size == 0)
		return;
	std::copy((Byte*)source, (Byte*)source + size, (Byte*)dest);
}

void MemZero(Pointer p, Cardinal size)
{
	if (size == 0)
		return;
	std::fill_n((Byte*)p, size, 0); 
}

}
}
