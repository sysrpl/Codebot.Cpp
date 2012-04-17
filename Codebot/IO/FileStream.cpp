#include "Codebot/IO/FileStream.h"
#include "Codebot/IO/IOExceptions.h"

namespace Codebot
{
namespace IO
{

using namespace std;

FileStream::FileStream(const String& fileName, FileModes mode)
	: name(fileName), buffer()
{
	try
	{
		#ifdef GCC
		auto m = fstream::binary;
		#else
		Integer m = fstream::binary;
		#endif
		if (mode.Contains(FileMode::Read))
			m = m | fstream::in;
		if (mode.Contains(FileMode::Write))
			m = m | fstream::out;
		if (mode.Contains(FileMode::Truncate))
			m = m | fstream::trunc;
		buffer =  MakeShared<std::fstream>(fileName, m);
	}
	catch (...)
	{
		buffer = null;
		ThrowFileStreamException(ThisMethod);
	}
}

FileStream::~FileStream()
{
	if (buffer->is_open())
		buffer->close();
}

Boolean FileStream::Eof()
{
	return buffer->eof();
}

const String FileStream::FileName() const
{
	return name;
}

void FileStream::Clear()
{
	static auto clear = fstream::binary | fstream::trunc | fstream::in | fstream::out;
	if (buffer->is_open())
		buffer->close();
	try
	{
		buffer = MakeShared<std::fstream>(name, clear);
	}
	catch (...)
	{
		buffer = null;
		ThrowFileStreamException(ThisMethod);
	}
}


Cardinal FileStream::Read(Pointer data, Cardinal count)
{
	if (count == 0)
		return count;
	if (data == null)
		ThrowNullReferenceException(ThisMethod, "data");
	if (buffer->eof())
		return 0;
	buffer->clear();
	buffer->read((Char*)data, count);
	Boolean failed = buffer->eof() ? false : buffer->fail();
	buffer->seekp(buffer->tellg(), ios_base::beg);
	if (failed)
		ThrowFileStreamException(ThisMethod);
	auto read = buffer->gcount();
	return read < 0 ? 0 : (Cardinal)read;
}

Cardinal FileStream::Write(Pointer data, Cardinal count)
{
	if (count == 0)
		return count;
	if (data == null)
		ThrowNullReferenceException(ThisMethod, "data");
	buffer->clear();
	buffer->write((Char*)data, count);
	Boolean failed = buffer->fail();
	buffer->seekg(buffer->tellp(), ios_base::beg);
	if (failed)
		ThrowFileStreamException(ThisMethod);
	return count;
}

Cardinal FileStream::Seek(StreamOrigin origin, Integer cursor)
{
	switch (origin)
	{
		case StreamOrigin::Begin:
			buffer->seekg(cursor, ios_base::beg);
			break;
		case StreamOrigin::Current:
			buffer->seekg(cursor, ios_base::cur);
			break;
		case StreamOrigin::End:
			buffer->seekg(cursor, ios_base::end);
			break;
	}
	auto p = buffer->tellg();
	buffer->seekp(p, ios_base::beg);
	return p < 0 ? 0 : (Cardinal)p;
}

// Stream loading and saving

void StreamSave(Stream* stream, const String& fileName)
{
	if (stream == null)
		ThrowNullReferenceException(ThisMethod, "stream");
	auto fileStream = New<FileStream>(fileName, FileMode::Create);
	auto position = stream->Position();
	stream->Position(0);
	fileStream->Assign(stream);
	stream->Position(position);
}

void StreamLoad(Stream* stream, const String& fileName)
{
	if (stream == null)
		ThrowNullReferenceException(ThisMethod, "stream");
	if (stream == null)
		ThrowNullReferenceException(ThisMethod, "stream");
	auto fileStream = New<FileStream>(fileName, FileMode::Open);
	stream->Clear();
	stream->Assign(fileStream);
	stream->Position(0);
}

}
}
