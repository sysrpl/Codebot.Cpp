#pragma once
#include "Codebot/String.h"

namespace Codebot
{
namespace Net
{

class Url : public ValueType, public ISerializable
{
private:
	String data;
public:
	Url(const String& s);
	operator String () const;
	Boolean operator == (const Url& url) const;
	Boolean operator != (const Url& url) const;
	String Protocol() const;
	String Domain() const;
	Word Port() const;
	String Resource() const;
	Boolean Valid() const;
	// Url override methods
	Boolean Equals(ValueType* instance) const;
	// ISerializable
	String ToFormat(const String& format) const;
	Boolean FromFormat(const String& value, const String& format);
};

}
}
