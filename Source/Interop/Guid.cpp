#include <Codebot/Interop/Guid.h>
#include <Codebot/Memory/Buffer.h>
#include <Codebot/Memory/MemoryTools.h>

namespace Codebot
{
namespace Interop
{

using namespace Memory;

extern "C" import void GuidCreate(GuidBytes& guidBytes);

Guid Guid::Generate()
{
	Guid g;
	GuidCreate(g.guidBytes);
	return g;
}

Guid::Guid()
{
}

Guid::Guid(const PChar s)
{
	static Byte order[16] = 
		{3, 2, 1, 0, 5, 4, 7, 6, 8, 9, 10, 11, 12, 13, 14, 15};
	if (s == null)
		return;
	Integer i = 0;
	while (i < 32)
	{
		Byte* b = &guidBytes.Data[order[i >> 1]];
		Char c = *s++;
		if (c == 0)
			return;
		if (c >= '0' && c <= '9')
			*b = IsOdd(i++) ? *b | (c - '0') : (c - '0') << 0x4;
		else if (c >= 'A' && c <= 'F')
			*b = IsOdd(i++) ? *b | (c - 'A' + 0xA) : (c - 'A' + 0xA) << 0x4;
	}
}

Guid::operator const GuidBytes& () const
{
	return guidBytes;
}

const GuidBytes& Guid::operator ()() const
{
	return guidBytes;
}

Boolean Guid::operator == (const Guid& guid) const
{
	return MemCompare(guidBytes, guid.guidBytes);
}

Boolean Guid::Equals(ValueType* instance) const
{
	return ValuesEqual(this, instance);
}

// Guid::ISerializable

String Guid::ToFormat(const String& format) const
{
	String a = Hex::Encode(VarBytes(guidBytes), sizeof(guidBytes));
	String b = a;
	static Byte order[] = 
		{6, 7, 4, 6, 2, 3, 0, 1, 
		10, 11, 8, 9, 
		14, 15, 12, 13,
		18, 19, 16, 17};
	for (Integer i = 0; i < 20; i++)
		a[i] = b[order[i]];
	return String("{a-b-c-d-e}")
		.Replace("a", a.SubString(0, 8))
		.Replace("b", a.SubString(8, 4))
		.Replace("c", a.SubString(12, 4))
		.Replace("d", a.SubString(16, 4))
		.Replace("e", a.SubString(20, 12));
}

Boolean Guid::FromFormat(const String& value, const String& format)
{
	Guid g(value);
	*this = g;
	return true;
}

}
}
