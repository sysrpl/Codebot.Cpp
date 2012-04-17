#pragma once
#include "Codebot/Types.h"

namespace Codebot
{
namespace Interop
{

extern "C" 
{	
	import Integer AtomicIncrement(Integer& i);
	import Integer AtomicDecrement(Integer& i);
	import Integer CommandLineCount();
	import PChar CommandLineArgument(Integer index);
	import Boolean IntegerParse(const PChar s, Integer& value);
	import Boolean SingleParse(const PChar s, Single& value);
	import Boolean DoubleParse(const PChar s, Double& value);
};

}
}
