#pragma once
#include <Codebot/Stream.h>
#include <Codebot/Memory/Buffer.h>

namespace Codebot
{
namespace Memory
{

class MemoryStream : public Stream
{
private:
	Buffer buffer;
	Cardinal position;
public:
	MemoryStream();
	operator Byte* () const;
	// Override methods
	void Clear();
	Cardinal Read(Pointer data, Cardinal count);
	Cardinal Write(Pointer data, Cardinal count);
	Cardinal Seek(StreamOrigin origin, Integer cursor);
	Cardinal Size() const;
	void Size(Cardinal value);
	Byte* Data() const;
};

}
}
