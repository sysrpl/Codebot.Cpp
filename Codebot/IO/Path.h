#pragma once
#include "Codebot/String.h"

namespace Codebot
{
namespace IO
{

class Path
{
private:
	Path();
public:
	static const String Separator;
	static String Parent(const String& a);
	static String Combine(const String& a, const String& b);
	static Boolean Exists(const String& path);
};

}
}
