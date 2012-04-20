#include <Codebot/Utility/FileUtility.h>

using namespace Codebot;
using namespace Codebot::Expressions;
using namespace Codebot::IO;
using namespace Codebot::Text;
using namespace Codebot::Utility;

void ShowUsage()
{
	String name = File::Name(Application::Name());
	WriteLine("{0} usage:", name);
	WriteLine("{0} {1}file:<file list> {1}pattern:<search pattern> "
		"[{1}path:<path, default = current>] [{1}recurse:<y|n, default = y>]", name, SwitchChar);
	WriteLine("example: {0} {1}file:*.h;*.cpp {1}pattern:return", name, SwitchChar);
}

void Search()
{
	String dir;
	if (!Application::FindSwitch("dir:", dir))
		dir = Directory::Current();
	if (!Directory::Exists(dir))
	{
		WriteLine("Directory '{0}' was not found", dir);
		return;
	}
	String file;
	Application::FindSwitch("file:", file);
	if (file.IsEmpty())
	{
		ShowUsage();
		return;
	}
	String pattern;
	Application::FindSwitch("pattern:", pattern);
	if (pattern.IsEmpty())
	{
		ShowUsage();
		return;
	}
	String recurse;
	Application::FindSwitch("recurse:", recurse);
	FileSearch(dir, file, pattern, recurse != "n");
}

int main()
{
	return Application::Run(Search);
}
