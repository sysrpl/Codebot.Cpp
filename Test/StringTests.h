#include "Init.h"

namespace StringTestsSpace
{
	String a;
	String b;
}

void StringTests(UnitTest& tests)
{
	using namespace StringTestsSpace;

	b = "this is|a test|of items";
	AddTestGroup("String tests");
	AddTestItem("empty",
		a = "";
		return a.IsEmpty();
	);
	AddTestItem("ctor(pchar)",
		return !(a = "Abc").IsEmpty();
	);
	AddTestItem("ctor(string)",
		return !(a = a).IsEmpty();
	);
	AddTestItem("equal",
		return a == "Abc";
	);
	AddTestItem("less than",
		return a < "abc";
	);
	AddTestItem("less than or equal",
		return a < "abc";
	);
	AddTestItem("greater than",
		return a > "ABC";
	);
	AddTestItem("greater than or equal",
		return a >= "ABC";
	);
	AddTestItem("length",
		return a.Length() == 3;
	);
	AddTestItem("const []",
		return a[2] == 'c';
	);
	AddTestItem("& []",
		a[2] = 'd'; 
		Boolean r = a[2] == 'd'; 
		a[2] = 'c'; 
		return r;
	);
	AddTestItem("+ string",
		return a + "123" == "Abc123";
	);
	AddTestItem("string +",
		return "123" + a == "123Abc";
	);
	AddTestItem("+ char",
		return a + '1' == "Abc1";
	);
	AddTestItem("char +",
		return '1' + a == "1Abc";
	);
	AddTestItem("toupper",
		return a.ToUpper() == "ABC";
	);
	AddTestItem("tolower",
		return a.ToLower() == "abc";
	);
	AddTestItem("+= char",
		return (a += '1') == "Abc1";
	);
	AddTestItem("+= pchar",
		return (a += "23") == "Abc123";
	);
	AddTestItem("+= string",
		return (a += a) == "Abc123Abc123";
	);
	AddTestItem("replace",
		return a.Replace("Abc", "a") == "a123a123";
	);
	AddTestItem("replaceone",
		return a.ReplaceOne("Abc", "a") == "a123Abc123";
	);
	AddTestItem("indexof -1",
		return a.IndexOf("X") == -1 ;
	);
	AddTestItem("indexof string",
		return a.IndexOf("3A") ==5 ;
	);
	AddTestItem("indexof char",
		return a.IndexOf('3') == 5;
	);
	AddTestItem("beginswith",
		return a.BeginsWith("Abc1");
	);
	AddTestItem("endswith",
		return a.EndsWith("123");
	);
	AddTestItem("firstof",
		return b.FirstOf(" is|") == "this";
	);
	AddTestItem("!firstof",
		return b.FirstOf(" isz|") == String::Empty;
	);
	AddTestItem("secondof",
		return b.SecondOf("|") == "a test|of items";
	);
	AddTestItem("!secondof",
		return b.SecondOf("|zz") == String::Empty;
	);
	AddTestItem("lastof",
		return b.LastOf("|of ") == "items";
	);
	AddTestItem("!lastof",
		return b.LastOf("|of z") == String::Empty;
	);
	AddTestItem("containsany",
		return b.ContainsAny("test");
	);
	AddTestItem("!containsany",
		return !b.ContainsAny("zyw");
	);
	AddTestItem("contains",
		return b.Contains("test|");
	);
	AddTestItem("!contains",
		return !b.Contains("testz");
	);
	AddTestItem("firstindexof",
		return b.FirstIndexOf(" is") == 4;
	);
	AddTestItem("!firstindexof",
		return b.FirstIndexOf(" isa") == -1;
	);
	AddTestItem("lastindexof",
		return b.FirstIndexOf(" is") == 4;
	);
	AddTestItem("lastindexof",
		return b.LastIndexOf("|") == 14;
	);
	AddTestItem("!lastindexof",
		return b.LastIndexOf("|z") == -1;
	);
	AddTestItem("splitone",
		auto c = b.SplitOne(" i");
		return c.Length() == 2 && c[0] == "this"
			&& c[1] == "s|a test|of items";
	);
	AddTestItem("!splitone",
		auto c = b.SplitOne(" ix");
		return c.Length() == 1 && c[0] == b;
	);
	AddTestExcept("Esubstring",
		b.SubString(-1, 1);
	);
	AddTestNotExcept("Esubstring",
		b.SubString(0, 100);
	);
}
