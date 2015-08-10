#pragma once
#include <Codebot/Exception.h>

namespace Codebot
{
namespace Interop
{

void ThrowModuleLoadException(const String& method, const String& module);
void ThrowModuleLoadProcException(const String& method, 
	const String& module, const String& procName);

class ModuleException : public Exception
{
public:
	ModuleException(const String& method, const String& message)
		: Exception(method, message) { }
};

}
}
