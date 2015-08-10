#include <Codebot/IO/File.h>
#include <Codebot/IO/Path.h>
#include <Codebot/Interop/FileInterop.h>

namespace Codebot
{
namespace IO
{

using namespace Interop;

File::File()
{
}

String File::Path(const String& sourceFile)
{
	return Path::Parent(sourceFile);
}

String File::FileName(const String& sourceFile)
{
	auto i = sourceFile.LastIndexOf(Path::Separator);
	return i < 0 ? sourceFile : sourceFile.SubString(i + 1);
}

String File::Name(const String& sourceFile)
{
	auto s = FileName(sourceFile);
	auto i = s.LastIndexOf(".");
	return i < 0 ? s : s.SubString(0, i);
}

String File::Extension(const String& sourceFile)
{
	auto s = FileName(sourceFile);
	auto i = sourceFile.LastIndexOf(".");
	return i < 0 ? String::Empty : sourceFile.SubString(i);
}

Boolean File::Delete(const String& fileName)
{
	return FileDelete(fileName);
}

Boolean File::Rename(const String& oldName, const String& newName)
{
	return FileRename(oldName, newName);
}

Boolean File::Exists(const String& fileName)
{
	return FileExists(fileName);
}

}
}
