#include "Codebot/Memory/MemoryStream.h"
#include "Codebot/Memory/MemoryTools.h"

namespace Codebot
{
namespace Memory
{

MemoryStream::MemoryStream()
	: buffer(), position(0)
{
}

MemoryStream::operator Byte* () const
{
	return buffer;
}

void MemoryStream::Clear()
{
	buffer.Size(0);
	position = 0;
}

Cardinal MemoryStream::Read(Pointer data, Cardinal count)
{
	if (count == 0)
		return count;
	if (data == null)
		ThrowNullReferenceException(ThisMethod, "data");
	Cardinal read = count;
	if (count + position > buffer.Size())
		read = buffer.Size() - position;
	if (read == 0)
		return read;
	MemCopy((Byte*)buffer + position, data, read);
	position += read;
	return read;
}

Cardinal MemoryStream::Write(Pointer data, Cardinal count)
{
	if (count == 0)
		return count;
	if (data == null)
		ThrowNullReferenceException(ThisMethod, "data");
	Cardinal written = count;
	if (position + count > buffer.Size())
		buffer.Size(position + count);
	Byte* source = (Byte*)data;
	Byte* dest = (Byte*)buffer + position;
	std::copy(source, source + written, dest);
	position += written;
	return written;
}

Cardinal MemoryStream::Seek(StreamOrigin origin, Integer cursor)
{
	Cardinal count = buffer.Size();
	switch (origin)
	{
		case StreamOrigin::Begin:
			if (cursor < 0)
				position = 0;
			else if (cursor > (Integer)count)
				position = count;
			else
				position = cursor;
			break;
		case StreamOrigin::Current:
			if (position + cursor < 0)
				position = 0;
			else if (position + cursor > count)
				position = count;
			else
				position += cursor;
			break;
		case StreamOrigin::End:
			if (cursor > (Integer)count)
				position = 0;
			else if (cursor < 0)
				position = count;
			else
				position = count - cursor;
			break;
	}
	return position;
}

Cardinal MemoryStream::Size() const
{
	return buffer.Size();
}

void MemoryStream::Size(Cardinal value)
{
	buffer.Size(value);
	if (value < position)
		position = value;
}

Byte* MemoryStream::Data() const
{
	return buffer;
}

}
}
