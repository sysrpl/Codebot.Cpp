#include "Codebot/Text/Xml/XmlExceptions.h"

namespace Codebot
{
namespace Text
{
namespace Xml
{

const String SInvalidName = "The name \"{0}\" is not a valid identifier";

void ThrowXmlNameException(const String& method, const String& name)
{
	throw XmlNameException(method, Format(SInvalidName, name));
}

}
}
}
