#pragma once
#include <Codebot/Exception.h>

namespace Codebot
{
namespace Game
{

void ThrowVideoInitializeException(const String& method);

class VideoInitializeException : public Exception
{
public:
	VideoInitializeException(const String& method, const String& message)
		: Exception(method, message) { }
};

}
}
