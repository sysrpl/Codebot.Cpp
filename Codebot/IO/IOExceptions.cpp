#include "Codebot/IO/IOExceptions.h"

namespace Codebot
{
namespace IO
{

const PChar SFileStreamFailed = "A FileStream method failed";

void ThrowFileStreamException(const String& method)
{
	throw FileStreamException(method, SFileStreamFailed);
}

}
}
