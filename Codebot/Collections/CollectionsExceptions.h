#pragma once
#include <Codebot/Exception.h>
#include <Codebot/Text/Format.h>

namespace Codebot
{
namespace Collections
{

void ThrowDuplicateItemException(const String& method);
void ThrowNoKeyException(const String& method);

class CollectionItemException : public Exception
{
public:
	CollectionItemException(const String& method, const String& message)
		: Exception(method, message) { }
};

}
}
