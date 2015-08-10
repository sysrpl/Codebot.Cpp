#pragma once
#include <Codebot/String.h>

namespace Codebot
{
namespace IO
{

class File
{
private:
	File();
public:
	static String Path(const String& sourceFile);
	static String FileName(const String& sourceFile);
	static String Name(const String& sourceFile);
	static String Extension(const String& sourceFile);
	static Boolean Delete(const String& fileName);
	static Boolean Rename(const String& oldName, const String& newName);
	static Boolean Exists(const String& fileName);
};

}
}
