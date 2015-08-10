#include <Codebot/Packages/System.h>
#include <Codebot/Packages/Expressions.h>

namespace Codebot
{
namespace Utility
{

// TODO: Write documentation

void ForEachLine(const String& fileName, Func<Boolean(String&)> func);
void ForEachFile(const String& path, const String& wildcards, Func<void(const String&)> func);
void ForEachFile(const String& path, Func<void(const String&)> func);
void ForEachFolder(const String& path, Func<void(const String&)> func);
void ForEveryLine(const String& path, const String& wildcards, Func<Boolean(const String&, String&)> func,
	Boolean recursive = true);

void FileSearch(const String& path, const String& wildcards, const String& pattern,
	Boolean recursive = true);

}
}

