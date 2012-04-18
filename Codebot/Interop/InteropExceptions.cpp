#include <Codebot/Interop/InteropExceptions.h>

namespace Codebot
{
namespace Interop
{

using namespace Text;

const PChar SNoLoadModule = "Could not load module {0}";
const PChar SNoFindProc = "Could not find proc {0}.{1}";

void ThrowModuleLoadException(const String& method, const String& module)
{
	throw ModuleException(method, Format(SNoLoadModule, module));
}

void ThrowModuleLoadProcException(const String& method, 
	const String& module, const String& procName)
{
	throw ModuleException(method, Format(SNoFindProc, module, procName));
}

}
}
