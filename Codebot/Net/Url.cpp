#include <Codebot/Net/Url.h>

namespace Codebot
{
namespace Net
{

// Url

Url::Url(const String& s)
	: data(s)
{
}

Url::operator String () const
{
	return data;
}
Boolean Url::operator == (const Url& url) const
{
	return data == url.data;
}
Boolean Url::operator != (const Url& url) const
{
	return data == url.data;
}

String Url::Protocol() const
{
	auto a = data.IndexOf("://");
	if (a < 0)
		return "http";
	else
		return data.SubString(0, a);
}

String Url::Domain() const
{
	auto a = data.IndexOf("://");
	if (a < 0)
		a = 0;
	else
		a += 3;
	auto b = data.IndexOf("/",  a);
	auto c = data.IndexOf(":",  a);
	if (b == c)
		return data.SubString(a);
	return c > -1  ? data.SubString(a, c - a) : data.SubString(a, b - a);
}

Word Url::Port() const
{
	auto a = data.IndexOf("://");
	if (a < 0)
		a = 0;
	else
		a += 3;
	auto b = data.IndexOf(":",  a);
	if (b > -1)
		return StrToInt(data.SubString(b + 1).FirstOf("/"), 80);
	else
		return 80;
}

String Url::Resource() const
{
	auto a = data.IndexOf("://");
	if (a < 0)
		a = 0;
	else
		a += 3;
	return "/" + data.SubString(a).SecondOf("/");
}

Boolean Url::Valid() const
{
	return !Domain().IsEmpty();
}

// Url override methods

Boolean Url::Equals(ValueType* instance) const
{
	String s = data;
	return ValuesEqual(&s, instance);
}

// Url::ISerializable

String Url::ToFormat(const String& format) const
{
	return data;
}

Boolean Url::FromFormat(const String& value, const String& format)
{
	data = value;
	return true;
}

}
}
