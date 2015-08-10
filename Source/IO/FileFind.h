#pragma once
#include <Codebot/Flags.h>
#include <Codebot/String.h>
#include <Codebot/Interop/Interface.h>
#include <Codebot/Interop/FileInterop.h>
#include <Codebot/Time/DateTime.h>

namespace Codebot
{
namespace IO
{

// TODO: Eventually use strong enum class after MSVC gets support

enum FileAttribute
{
	File = 0,
	ReadOnly = 1,
	Hidden = ReadOnly << 1,
	SystemFile = Hidden << 1,
	VolumeID = SystemFile << 1,
	Directory = VolumeID << 1,
	Archive = Directory << 1,
	SymbolicLink = Archive << 1,
	AnyFile = 0x3F
};

typedef Flags<FileAttribute> FileAttributes;

// FileResult class

class FileResult : public ValueType
{
public:
	String Name;
	Cardinal Size;
	Time::DateTime Date;
	FileAttributes Attributes;
	// Override methods
	String ToFormat(const String& format) const;
};

class FileFind : ValueType, IEnumerable<FileResult>
{
private:
	Interop::Interface<Interop::IFileFind> find;
public:
	FileFind();
	void Search(const String& wildcard,
		FileAttributes attributes = FileAttribute::AnyFile);
	void Search(const String& path, const String& wildcard,
		FileAttributes attributes = FileAttribute::AnyFile);
	Boolean Next();
	// Properties
	const FileResult Result() const;
	// IEnumerable<FileResult>
	IEnumerator<FileResult>* GetEnumerator() const;
};

}
}
