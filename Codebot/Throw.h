#pragma once
#include "Codebot/Types.h"

namespace Codebot
{

void Abort();
void ThrowCopyObjectException(const PChar method);
void ThrowException(const PChar method, const PChar message);
void ThrowCastException(const PChar method, const PChar type);
void ThrowAssignException(const PChar method, const PChar source, const PChar dest);
void ThrowArgumentException(const PChar method, const PChar argument);
void ThrowNullReferenceException(const PChar method, const PChar ident);
void ThrowRangeException(const PChar method);

}
