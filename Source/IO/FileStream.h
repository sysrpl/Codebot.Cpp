#pragma once
#include <fstream>
#include <Codebot/Stream.h>
#include <Codebot/Flags.h>

namespace Codebot
{
namespace IO
{

// TODO: Eventually use strong enum class after MSVC gets support

enum FileMode
{
	Read = 1,
	Write = Read << 1,
	Truncate = Write << 1,
	Open = Read | Write,
	Create = Truncate | Open
};

typedef Flags<FileMode> FileModes;

class FileStream : public Stream
{
private:
	String name;
	Shared<std::fstream> buffer;
public:
	FileStream(const String& fileName, FileModes mode = FileMode::Open);
	~FileStream();
	const String FileName() const;
	Boolean Eof();
	// Override methods
	void Clear();
	Cardinal Read(Pointer data, Cardinal count);
	Cardinal Write(Pointer data, Cardinal count);
	Cardinal Seek(StreamOrigin origin, Integer cursor);
};

// Stream loading and saving

void StreamSave(Stream* stream, const String& fileName);
void StreamLoad(Stream* stream, const String& fileName);

}
}
