#pragma once
#include <Codebot/String.h>

namespace Codebot
{
namespace Interop
{

struct GuidBytes
{
	Byte Data[16];
};

class Guid : public ValueType, public ISerializable
{
private:
	GuidBytes guidBytes;
public:
	static Guid Generate();
	Guid();
	Guid(const PChar s);
	Boolean operator == (const Guid& guid) const;
	operator const GuidBytes& () const;
	const GuidBytes& operator ()() const;
	// Override methods
	Boolean Equals(ValueType* instance) const;
	// ISerializable
	String ToFormat(const String& format) const;
	Boolean FromFormat(const String& value, const String& format);
};

}
}
