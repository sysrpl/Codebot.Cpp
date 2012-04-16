#pragma once
#include "Codebot/String.h"
#include "Codebot/Memory/MemoryTools.h"

namespace Codebot
{
namespace Memory
{

typedef std::vector<Byte> Bytes;

class Buffer : public ValueType, public ISerializable
{
private:
	Shared<Bytes> data;
public:
	Buffer();
	Buffer(const Bytes& bytes);
	Buffer(Cardinal size);
	Buffer(Byte* buffer, Cardinal size);
	operator Byte* () const;
	Buffer Copy();
	Cardinal Size() const;
	void Size(Cardinal value);
	// ISerializable
	String ToFormat(const String& format) const;
	Boolean FromFormat(const String& value, const String& format);
};

template <typename T>
Buffer AnyToBuffer(const T& value)
{
	Buffer b(sizeof(T));
	MemCopy(&Var(value), b, sizeof(T));
	return b;
}

// String encoding routines

namespace Hex
{
	String Encode(Byte* buffer, Cardinal size);
	String Encode(const Buffer& buffer);
	Buffer Decode(const String& s);
}

namespace Base64
{
	String Encode(Byte* buffer, Cardinal size);
	String Encode(const Buffer& buffer);
	Buffer Decode(const String& s);
}

}
}
