#pragma once
#include <Codebot/String.h>

namespace Codebot
{

extern String SwitchChar;

class Application
{
private:
	Application();
public:
	static Integer Run(Proc proc);
	static Integer Run(const Array<Proc>& procs);
	static Integer FindSwitch(const String& name);
	static Boolean FindSwitch(const String& name, String& value);
	static Boolean FindSwitch(const String& name, Integer& value);
	static String Name();
	static const Array<String> Args();
};

}
