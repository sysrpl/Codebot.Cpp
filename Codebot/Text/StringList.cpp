#include <Codebot/Text/StringList.h>
#include <Codebot/Text/StringStream.h>
#include <Codebot/IO/FileStream.h>

namespace Codebot
{
namespace Text
{

using namespace IO;

StringList::StringList()
{
}

String StringList::Text() const
{
	String s;
	Integer length = Length() - 1;
	for (Integer i = 0; i < length; i++)
	{
		s += Item(i);
		s += String::NewLine;
	}
	if (length > -1)
		s += Item(length);
	return s;
}

void StringList::Text(const String& value)
{
	static String n = "\n";
	static String r = "\r";
	Array<String> items;
	if (value.Contains(n))
		items = value
			.Replace(r, String::Empty)
			.Split(n);
	else
		items = value
			.Replace(n, String::Empty)
			.Split(r);
	Integer length = items.Length();
	Clear();
	Capacity(length);
	for (Integer i = 0; i < length; i++)
		Push(items[i]);
	Changed();
}

String StringList::ToFormat(const String& format) const
{
	return Text().ToFormat(format);
}

Boolean StringList::FromFormat(const String& value, const String& format)
{
	String s;
	if (s.FromFormat(value, format))
	{
		Text(s);
		return true;
	}
	return false;
}

// StringList::IPersistent

void StringList::Load(const String& fileName)
{
	Load(New<FileStream>(fileName, FileMode::Open));
}

void StringList::Load(Stream* stream)
{
	if (stream == null)	
		ThrowNullReferenceException(ThisMethod, "stream");
	Ref<StringStream> s = new StringStream();
	s->Assign(stream);
	Text(*s);
}

void StringList::Save(const String& fileName) const
{
	Save(New<FileStream>(fileName, FileMode::Create));
}

void StringList::Save(Stream* stream) const
{
	if (stream == null)	
		ThrowNullReferenceException(ThisMethod, "stream");
	String s = Text();
	if (!s.IsEmpty())
		stream->Write(&s[0], s.Length());
}

}
}
