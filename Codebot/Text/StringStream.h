#pragma once
#include "Codebot/Stream.h"
#include "Codebot/Memory/Buffer.h"

namespace Codebot
{
namespace Text
{

// StringStream class

class StringStream : public Stream
{
private:
	String buffer;
	Cardinal position;
public:
	StringStream();
	operator String () const;
	// Override methods
	void Clear();
	Cardinal Read(Pointer data, Cardinal size);
	Cardinal Write(Pointer data, Cardinal size);
	Cardinal Seek(StreamOrigin origin, Integer cursor);
	Cardinal Size() const;
	void Size(Cardinal value);
	// ISerializable
	String ToFormat(const String& format) const;
	Boolean FromFormat(const String& value, const String& format);
};

}
}
