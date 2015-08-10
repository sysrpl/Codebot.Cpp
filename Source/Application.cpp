#include <Codebot/Application.h>
#include <Codebot/Exception.h>
#include <Codebot/Console.h>
#include <Codebot/Interop/CoreInterop.h>

namespace Codebot
{

String SwitchChar = "-";
String applicationName;
Array<String> commandLine;

void InitApplication()
{
	static Boolean initialized = false;
	if (initialized)
		return;
	applicationName = Interop::CommandLineArgument(0);
	Array<String> a;
	Integer length = Interop::CommandLineCount();
	for (Integer i = 1; i < length; i++)
		a.Add(Interop::CommandLineArgument(i));
	commandLine = a;
	initialized = true;
}

Application::Application()
{
}

Integer Application::Run(Proc proc)
{
	Integer error = 1;
	try
	{
		proc();
		error = 0;
	}
	catch (Exception& e)
	{
		WriteLine(e.ToString());
	}
	catch (std::exception& e)
	{
		WriteLine("std::exception {0}", String(e.what()));
	}
	catch (...)
	{
		WriteLine("Unknown exception");
	}
	return error;
}

Integer Application::Run(const Array<Proc>& procs)
{
	Integer length = procs.Length();
	for (Integer i = 0; i < length; i++)
	{
		Integer r = Run(procs[i]);
		if (r != 0)
			return r;
	}
	return 0;
}

Integer Application::FindSwitch(const String& name)
{
	if (name.IsEmpty())
		return -1;
	InitApplication();
	Integer length = commandLine.Length();
	for (Integer i = 0; i < length; i++)
	{
		String s = commandLine[i];
		if (s.BeginsWith(SwitchChar + name))
			return i;
	}
	return -1;
}

Boolean Application::FindSwitch(const String& name, Integer& value)
{
	auto i = FindSwitch(name);
	if (i < 0)
		return false;
	String s = commandLine[i].SubString(SwitchChar.Length() + name.Length());
	return Interop::IntegerParse(s, value);
}

Boolean Application::FindSwitch(const String& name, String& value)
{
	auto i = FindSwitch(name);
	if (i < 0)
		return false;
	value = commandLine[i].SubString(SwitchChar.Length() + name.Length());
	return true;
}

const Array<String> Application::Args()
{
	InitApplication();
	return commandLine;
}

String Application::Name()
{
	InitApplication();
	return applicationName;
}

}
