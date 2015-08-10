#include <Codebot/Exception.h>

namespace Codebot
{

using namespace Text;

Exception::Exception(const String& method, const String& message)
	: a(method), b(message)
{
}

String Exception::ToFormat(const String& format) const
{
	String s = format.IsEmpty() ? "exc: 'msg' in method 'mhd'" : format;
	s = s.Replace("exc", "{0}").Replace("mhd", "{1}").Replace("msg", "{2}");
	return Format(s, ClassName(), a, b);
}

const String Exception::Method() const
{
	return a;
}

const String Exception::Message() const
{
	return b;
}

/* SystemException */

const Integer SystemException::Code() const
{
	return c;
}

}
