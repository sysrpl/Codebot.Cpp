#include <Codebot/IO/Directory.h>
#include <Codebot/Interop/FileInterop.h>

namespace Codebot
{
namespace IO
{

using namespace Interop;

Directory::Directory()
{
}

String Directory::Current()
{
	return DirectoryCurrent();
}

Boolean Directory::Change(const String& dir)
{
	return DirectoryChange(dir);
}

Boolean Directory::Create(const String& dir)
{
	return DirectoryCreate(dir);
}

Boolean Directory::Delete(const String& dir)
{
	return DirectoryDelete(dir);
}

Boolean Directory::Rename(const String& oldName, const String& newName)
{
	return DirectoryRename(oldName, newName);
}

Boolean Directory::Exists(const String& dir)
{
	return DirectoryExists(dir);
}

}
}
