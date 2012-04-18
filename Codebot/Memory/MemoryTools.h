#pragma once
#include <Codebot/Types.h>

namespace Codebot
{
namespace Memory
{

Boolean MemCompare(Pointer a, Pointer b, Cardinal size);
void MemCopy(Pointer source, Pointer dest, Cardinal size);
void MemZero(Pointer p, Cardinal size);

template <typename T>
inline Boolean MemCompare(const T& a, const T& b)
{
	return MemCompare(&Var(a), &Var(b), sizeof(T));
}

}
}
