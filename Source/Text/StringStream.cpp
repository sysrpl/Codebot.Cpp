#include <Codebot/Text/StringStream.h>
#include <Codebot/Memory/MemoryTools.h>

namespace Codebot
{
namespace Text
{

using namespace Memory;

StringStream::StringStream()
	: buffer(), position(0)
{
}

StringStream::operator String () const
{
	return buffer;
}

void StringStream::Clear()
{
	buffer = String::Empty;
	position = 0;
}

Cardinal StringStream::Read(Pointer data, Cardinal size)
{
	if (size == 0)
		return size;
	if (data == null)
		ThrowNullReferenceException(ThisMethod, "data");
	Cardinal read = size;
	if (size + position > buffer.Length())
		read = buffer.Length() - position;
	if (read == 0)
		return read;
	Char* source = &buffer[0] + position;
	Char* dest = (Char*)data;
	std::copy(source, source + read, dest);
	position += read;
	return read;
}

Cardinal StringStream::Write(Pointer data, Cardinal size)
{
	if (size == 0)
		return size;
	if (data == null)
		ThrowNullReferenceException(ThisMethod, "data");
	Cardinal written = size;
	if (position + size > buffer.Length())
		buffer.Length((Integer)(position + size));
	Char* source = (Char*)data;
	Char* dest = &buffer[0] + position;
	MemCopy(source, dest, written);
	position += written;
	return written;
}

Cardinal StringStream::Seek(StreamOrigin origin, Integer cursor)
{
	Cardinal size = buffer.Length();
	switch (origin)
	{
		case StreamOrigin::Begin:
			if (cursor < 0)
				position = 0;
			else if (cursor > (Integer)size)
				position = size;
			else
				position = cursor;
			break;
		case StreamOrigin::Current:
			if (position + cursor < 0)
				position = 0;
			else if (position + cursor > size)
				position = size;
			else
				position += cursor;
			break;
		case StreamOrigin::End:
			if (cursor > (Integer)size)
				position = 0;
			else if (cursor < 0)
				position = size;
			else
				position = size - cursor;
			break;
	}
	return position;
}

Cardinal StringStream::Size() const
{
	return buffer.Length();
}

void StringStream::Size(Cardinal value)
{
	buffer.Length((Integer)value);
	if (value < position)
		position = value;
}

String StringStream::ToFormat(const String& format) const
{
	return buffer.ToFormat(format);
}

Boolean StringStream::FromFormat(const String& value, const String& format)
{
	return buffer.FromFormat(value, format);
}

}
}
