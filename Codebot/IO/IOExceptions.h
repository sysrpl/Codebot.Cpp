#pragma once
#include "Codebot/Exception.h"
#include "Codebot/Text/Format.h"

namespace Codebot
{
namespace IO
{

void ThrowFileStreamException(const String& method);

class FileStreamException : public Exception
{
public:
	FileStreamException(const String& method, const String& message)
		: Exception(method, message) { }
};

}
}
