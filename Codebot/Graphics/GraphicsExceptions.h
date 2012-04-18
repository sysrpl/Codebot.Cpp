#pragma once
#include <Codebot/Exception.h>

namespace Codebot
{
namespace Graphics
{

void ThrowImageException(const String& method);

class ImageException : public Exception
{
public:
	ImageException(const String& method, const String& message)
		: Exception(method, message) { }
};

}
}
