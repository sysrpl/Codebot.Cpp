#include "Codebot/Stream.h"
#include "Codebot/Memory/Buffer.h"

namespace Codebot
{

using namespace Memory;

void Stream::Clear()
{
	Size(0);
}

Cardinal Stream::Position()
{
	return Seek(StreamOrigin::Current, 0);
}

void Stream::Position(Cardinal value)
{
	Seek(StreamOrigin::Begin, value);
}

Cardinal Stream::Size()
{
	Cardinal p = Seek(StreamOrigin::Current, 0);
	Cardinal r = Seek(StreamOrigin::End, 0);
	Seek(StreamOrigin::Begin, p);
	return r;
}

void Stream::Size(Cardinal value)
{
}

void Stream::Assign(Object* source)
{
	auto stream = As<Stream>(source);
	if (stream != null)
	{
		Cardinal size = 0xF000;
		Buffer buffer(size);
		auto position = stream->Position();
		stream->Position(0);
		while (true)
		{
			auto count = stream->Read(buffer, size);
			if (count == 0)
				break;
			Write(buffer, count);
		}
		stream->Position(position); 
	}
	else
		Object::Assign(source);
}

// Stream::ISerializable

String Stream::ToFormat(const String& format) const
{	
	Stream& stream = Var(*this);
	auto size = stream.Size();
	if (size == 0)
		return String::Empty;
	auto position = stream.Position();
	stream.Position(0);
	Buffer b(size);
	stream.Read(b, size);
	stream.Position(position);
	String s = b.ToFormat(format);
	return s;
}

Boolean Stream::FromFormat(const String& value, const String& format)
{
	Buffer b;
	b.FromFormat(value, format);
	Clear();
	Write(b, b.Size());
	Position(0);
	return true;
}

}
