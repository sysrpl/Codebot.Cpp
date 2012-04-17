#include "Init.h"
#include "ArrayTests.h"
#include "DictionaryTests.h"
#include "ListTests.h"
#include "StringTests.h"

void AllTests()
{
	UnitTest tests;
	ArrayTests(tests);
	DictionaryTests(tests);
	ListTests(tests);
	StringTests(tests);
	WriteLine(tests.Execute());
}

int main()
{
	return Application::Run(AllTests);
}
