#include <Codebot/Console.h>

namespace Codebot
{

Char ReadChar()
{
	return getchar();
}

String ReadLine(const String& defaultValue)
{
	std::string input = "";
	std::getline(std::cin, input);
	String s = (const PChar)input.c_str();
	return s.IsEmpty() ? defaultValue : s;
}

void Write(const String& s)
{
	std::cout << s.ToString();
}

void WriteLine(const String& s)
{
	std::cout << s.ToString() << std::endl;
}

}
