#include <Codebot/Utility/FileUtility.h>

namespace Codebot
{
namespace Utility
{

using namespace Codebot;
using namespace Codebot::Expressions;
using namespace Codebot::IO;
using namespace Codebot::Text;

void ForEachLine(const String& fileName, Func<Boolean(String&)> func)
{
	auto changed = false;
	auto lines = New<StringList>();
	lines->Load(fileName);
	Integer length = lines->Length();
	for (Integer i = 0; i < length; i++)
	{
		String& line = lines->Item(i);
		changed |= func(line);
	}
	if (changed)
		lines->Save(fileName);
}

void ForEachFile(const String& path, const String& wildcards, Func<void(const String&)> func)
{
	String s = wildcards.IsEmpty() ? "*" : wildcards;
	auto patterns = s.Split(";");
	ForEach(pattern, patterns)
	{
		FileFind find;
		find.Search(path, pattern, FileAttribute::File);
		ForEach(item, find)
			func(Path::Combine(path, item->Name));
	}
}

void ForEachFile(const String& path, Func<void(const String&)> func)
{
	ForEachFile(path, "*", func);
}

void ForEachFolder(const String& path, Func<void(const String&)> func)
{
	static const String current = ".";
	static const String parent = "..";
	FileFind find;
	find.Search(path, "*", FileAttribute::Directory);
	ForEach(item, find)
	{
		if (!item->Attributes.Contains(FileAttribute::Directory))
			continue;
		String s = item->Name;
		if ((s == current) || (s == parent))
			continue;
		func(Path::Combine(path, item->Name));
	}
}

void ForEveryLine(const String& path, const String& wildcards,
	Func<Boolean(const String&, String&)> func, Boolean recursive)
{
	ForEachFile(path, wildcards, [&] (String file)
	{
		ForEachLine(file, [&] (String& line)
		{
				return func(file, line);
		});
	});
	if (recursive)
		ForEachFolder(path, [&] (String folder)
		{
			ForEveryLine(folder, wildcards, func, recursive);
		});
}

void FileSearch(const String& path, const String& wildcards, const String& pattern, Boolean recursive)
{
	String last = String::Empty;
	Boolean changed = false;
	int i = 0;
	ForEveryLine(path, wildcards,
	[&] (String file, String& line) -> Boolean
	{
		if (last != file)
		{
			i = 0;
			last = file;
			changed = true;
		}
		i++;
		if (line.Contains(pattern))
		{
			if (changed)
				WriteLine("\nIn file {0}", file);
			changed = false;
			#ifdef WindowsOS
			WriteLine("line {0}: {1}", i, line);
			#else
			WriteLine("line {0}: {1}", i,
				line.Replace(pattern, "\033[1;32m" + pattern + "\033[0m"));
			#endif
		}
		return false;
	}, recursive);
}

}
}

