#include "Init.h"

namespace ListTestsSpace
{
	typedef List<String> Strings;

	Boolean FindZap(const String& s)
	{
		return s.BeginsWith("zap");
	}

	Boolean DontFindZap(const String& s)
	{
		return s.BeginsWith("Zap");
	}

	Boolean StringSearch(const String& a, const String& b)
	{
		return a < b;
	}

	Boolean LongSort(const String& a, const String& b)
	{
		return a.Length() > b.Length();
	}

	Ref<Strings> a;
	Ref<Strings> b;
}

void ListTests(UnitTest& tests)
{
	using namespace ListTestsSpace;

	a = New<Strings>();
	b = New<Strings>();
	AddTestGroup("List tests");
	AddTestItem("ctor()",
		return a->Length() == 0;
	);
	AddTestItem("add",
		a->Add("abc"); return a->Length() == 1;
	);
	AddTestItem("length",
		a->Add("123"); 
		return a->Length() == 2;
	);
	AddTestItem("const []",
		a->Add("zapper"); 
		return (*a)[1] == "123";
	);
	AddTestItem("ctor(copy)",
		b->Assign(a); 
		return (*b)[2] == "zapper"; 
	);
	AddTestItem("& []",
		(*a)[1] = "1234"; 
		return (*a)[1] == "1234";
	);
	AddTestItem("sort",
		a->Sort(); 
		return (*a)[1] == "abc";
	);
	AddTestItem("search(t)",
		return a->Search("zapper") == 2;
	);
	AddTestItem("search(t, c)",
		return a->Search("1234", StringSearch) == 0;
	);
	AddTestItem("indexof",
		return a->IndexOf(String("abc")) == 1;
	);
	AddTestItem("remove",
		a->Remove("abc"); 
		return a->IndexOf(String("abc")) < 0;
	);
	AddTestItem("find",
		return a->Find(FindZap) == 1;
	);
	AddTestItem("extract",
		a->Extract(FindZap); 
		return a->Length() == 1;
	);
	AddTestItem("clear",
		a->Clear();
		const Strings& s = a.Value();
		return s.Length() == 0;
	);
	AddTestItem("foreach",
		a->Clear();
		auto s = String("The rain in Spain falls mainly in the plains").Split(" ");
		ForEach(word, s)
			a->Add(word);
		return a->Length() == 9;
	);
	AddTestItem("sortfunc",
		a->Sort(LongSort);
		return a->Item(8) == "in";
	);
	AddTestItem("delete",
		auto s = a->Item(2);
		a->Delete(2);
		return (a->Item(2) != s) && (a->Length() == 8) && (a->Item(7) == "in");
	);
}
