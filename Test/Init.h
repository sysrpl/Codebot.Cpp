#pragma once
#include <Codebot/Packages/System.h>
#include <Codebot/Packages/Expressions.h>
#include <Codebot/Packages/Testing.h>

#define AddTestGroup(name) tests.DefineGroup(name)
#define AddTestItem(name, test) tests.DefineTest(name, [] () -> Boolean { test })
#define AddTestExcept(name, test) tests.DefineTest(name, [] () -> Boolean \
		{ try { test return false; } catch (...) { return true; }  })
#define AddTestNotExcept(name, test) tests.DefineTest(name, [] () -> Boolean \
		{ try { test return true; } catch (...) { return false; }  })

using namespace Codebot;
using namespace Codebot::Collections;
using namespace Codebot::Expressions;
using namespace Codebot::Text;
using namespace Codebot::Testing;

extern Array<Proc> Initializers;
extern Array<Proc> Finalizers;
