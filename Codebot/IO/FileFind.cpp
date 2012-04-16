#include "Codebot/IO/FileFind.h"
#include "Codebot/Object.h"
#include "Codebot/Text/Format.h"

namespace Codebot
{
namespace IO
{

using namespace Codebot::Interop;
using namespace Codebot::Text;
using namespace Codebot::Time;

// FileResult class

String FileResult::ToFormat(const String& format) const
{
	static const String columns = "columns";
	static const String directory = "{0|20} {1|-10}";
	static const String file = "{0|20} {1|-10}  {2:YYYY-MM-DD hh:mm:ss}";
	static const String dir = "Dir";
	String s = format.IsEmpty() ? columns : format;
	if (s == columns)
	{
		if (Attributes.Contains(FileAttribute::Directory))
			return Format(directory, Name, dir);
		else
			return Format(file, Name, Size, Date);
	}
	return String::Empty;
}
// FileFindEnumerator class

class FileFindEnumerator : public Object, public IEnumerator<FileResult>
{
private:
	Interface<IFileFind> find;
	Boolean hasNext;
	FileResult result;
public:
	FileFindEnumerator(IFileFind* find);
	// IEnumerator<FileResult>
	void Reset();
	Boolean HasNext();
	FileResult* Next();
};

FileFindEnumerator::FileFindEnumerator(IFileFind* f) :
	find(f), hasNext(false)
{
}

void FileFindEnumerator::Reset()
{
	find->Close();
	hasNext = false;
}

Boolean FileFindEnumerator::HasNext()
{
	if (!hasNext)
		hasNext = find->Next();
	return hasNext;
}

FileResult* FileFindEnumerator::Next()
{
	if (!hasNext)
		find->Next();
	result.Name = find->Name();
	result.Size = find->Size();
	result.Date = DateTime::FromInterop(find->Date());
	result.Attributes.Value(find->Attributes());
	hasNext = find->Next();
	return &result;
}

// FileFind class

FileFind::FileFind() :
	find(null)
{
	FileFindCreate(Out(find));
}

void FileFind::Search(const String& wildcard, FileAttributes attributes)
{
	find->Search(String::Empty, wildcard, attributes.Value());
}

void FileFind::Search(const String& path, const String& wildcard,
	FileAttributes attributes)
{
	find->Search(path, wildcard, attributes.Value());
}

Boolean FileFind::Next()
{
	return find->Next();
}

// FileFind properties

const FileResult FileFind::Result() const
{
	FileResult r;
	r.Name = find->Name();
	r.Size = find->Size();
	r.Date = DateTime::FromInterop(find->Date());
	r.Attributes.Value(find->Attributes());
	return r;
}

// FileFind::IEnumerable<FileResult>

IEnumerator<FileResult>* FileFind::GetEnumerator() const
{
	return new FileFindEnumerator(find);
}

}
}
