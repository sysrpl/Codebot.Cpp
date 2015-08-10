#pragma once
#include <Codebot/Packages/System.h>

namespace Codebot
{
namespace Testing
{

typedef Func<Boolean(void)> TestFunc;

class TestItem : public ValueType
{
public:
	String Name;
	TestFunc Test;
	TestItem(const String& name, TestFunc test);
};

typedef Collections::List<TestItem> TestItems;

class TestGroup : public ValueType
{
public:
	String Name;
	Ref<TestItems> Items;
	TestGroup(const String& name);
};

typedef Collections::List<TestGroup> TestGroups;

class UnitTest : public ValueType
{
private:
	Ref<TestGroups> groups;
public:
	UnitTest();
	void DefineGroup(const String& name);
	void DefineTest(const String& name, TestFunc test);
	String Execute();
};

}
}
