#include "Codebot/IO/Path.h"
#include "Codebot/IO/Directory.h"
#include "Codebot/IO/File.h"
#include "Codebot/Interop/FileInterop.h"

namespace Codebot
{
namespace IO
{

using namespace Interop;

#ifdef WindowsOS
const String Path::Separator = "\\";
#else
const String Path::Separator = "/";
#endif


Path::Path()
{
}

String Path::Parent(const String& a)
{
	auto i = a.LastIndexOf(Separator);
	if (i < 0)
		return a;
	return a.SubString(0, i);
}

String Path::Combine(const String& a, const String& b)
{
	return a.EndsWith(Separator) ? a + b : a + Separator + b;
}

Boolean Path::Exists(const String& path)
{
	return File::Exists(path) || DirectoryExists(path);
}

}
}
