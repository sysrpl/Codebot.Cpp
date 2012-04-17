#include "Init.h"

namespace ArrayTestsSpace
{
	Array<Integer> i;
	Array<String> s;
}

void ArrayTests(UnitTest& tests)
{
	using namespace  ArrayTestsSpace;

	AddTestGroup("Array tests");
	AddTestItem("ctor(int)",
		s = Array<String>(2);
		return s[0].Length() == 0;
	);
	AddTestItem("empty",
		return i.Length() + 2 == s.Length(); 
	);
	AddTestItem("add",
		i.Add(12); 
		return i[0] == 12;
	);
	AddTestItem("remove",
		i.Remove(12); 
		return i.Length() == 0;
	);
	AddTestItem("const []",
		s[1] = "hello"; 
		return s[1].Length() == 5;
	);
	AddTestItem("& []",
		s[1] = "abc"; 
		auto b = s[1] == "abc"; 
		return b;
	);
	AddTestItem("clear",
		s.Clear(); 
		return s.Length() == 0;
	);
	AddTestItem("foreach",
		Integer i = 0;
		s = String("The rain in Spain falls mainly in the plains").Split(" ");
		ForEach(word, s)
			i++;
		return i == 9;
	);
}
