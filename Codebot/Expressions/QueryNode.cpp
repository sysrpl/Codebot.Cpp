#include <Codebot/Expressions/QueryNode.h>

namespace Codebot
{
namespace Expressions
{

Boolean CodebotExpressions = false;

Boolean GetCodebotExpressions()
{
	CodebotExpressions = !CodebotExpressions;
	return CodebotExpressions;
}

}
}
