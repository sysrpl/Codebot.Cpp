#pragma once
#include "Codebot/String.h"

namespace Codebot
{
namespace IO
{

class Directory
{
private:
	Directory();
public:
	static String Current();
	static Boolean Change(const String& dir);
	static Boolean Create(const String& dir);
	static Boolean Delete(const String& dir);
	static Boolean Rename(const String& oldName, const String& newName);
	static Boolean Exists(const String& fileName);
};

}
}
