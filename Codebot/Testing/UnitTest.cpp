#include <Codebot/Testing/UnitTest.h>
#include <Codebot/Expressions/Query.h>

namespace Codebot
{
namespace Testing
{

using namespace Text;
using namespace Expressions;

// TestItem

TestItem::TestItem(const String& name, TestFunc test)
	: Name(name), Test(test)
{
}

// TestGroup

TestGroup::TestGroup(const String& name)
	: Name(name)
{
	Items = New<TestItems>();
}

// UnitTest

UnitTest::UnitTest()
{
	groups = New<TestGroups>();
}

void UnitTest::DefineGroup(const String& name)
{
	groups->Add(TestGroup(name));
}

void UnitTest::DefineTest(const String& name, TestFunc test)
{
	if (groups->IsEmpty())
		DefineGroup("Unknown Group");
	TestGroup& g = groups->Last();
	g.Items->Add(TestItem(name, test));
}

String UnitTest::Execute()
{
	auto fails = New<StringList>();
	auto errors = New<StringList>();
	auto lines = New<StringList>();
	lines->Add("Running tests");
	Integer passCount = 0;
	Integer failCount = 0;
	Integer errorCount = 0;
	ForEach(group, groups)
	{
		lines->Add("\nEntering Group: " + group->Name);
		ForEach(item, group->Items)
		{
			Boolean error = true;
			try
			{
				Boolean passed = item->Test();
				if (passed)
					passCount++;
				else
					failCount++;
				lines->Add(Format("{0|30}{1:p}", item->Name, passed));
				if (!passed)
					fails->Add(Format("{0}: {1}", group->Name, item->Name));
				error = false;
			}
			catch (const Exception& e)
			{
				errors->Add(Format("{0}: {1}\n  Thrown: {2:exc}\n  Message: {2:'msg' in 'mhd'}",
					group->Name, item->Name, e));
			}
			catch (const std::exception& e)
			{
				errors->Add(Format("{0}: {1}\n  Thrown: {2}\n  Message: '{3}'",
					group->Name, item->Name, TypeToStr(e), e.what()));
			}
			catch (...)
			{
				errors->Add(Format("{0}: {1}\n  Thrown: unknown exception\n",
					group->Name, item->Name));
			}
			if (error)
			{
				errorCount++;
				lines->Add(Format("{0|30}error", item->Name));
			}
		}
	}
	lines->Add("\nTest results:");
	lines->Add(Format("\n{0} passed\n{1} failed\n{2} errors",
		passCount, failCount, errorCount));
	if (fails->Length() > 0)
	{
		lines->Add("\nFailures:");
		ForEach(s, fails)
			lines->Add(s);
	}
	if (errors->Length() > 0)
	{
		lines->Add("\nErrors:");
		ForEach(s, errors)
			lines->Add(s);
	}
	return lines->Text();
}

}
}
