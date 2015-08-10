#pragma once
#include <Codebot/Exception.h>

namespace Codebot
{
namespace Text
{
namespace Xml
{

void ThrowXmlNameException(const String& method, const String& name);

class XmlException : public Exception
{
public:
	XmlException(const String& method, const String& message)
		: Exception(method, message) { }
};

class XmlNameException : public XmlException
{
public:
	XmlNameException(const String& method, const String& message)
		: XmlException(method, message) { }
};

}
}
}
