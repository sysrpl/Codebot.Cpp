#include <Codebot/Game/GameExceptions.h>

namespace Codebot
{
namespace Game
{

const String SVideoFailed = "The video sub system failed to initialize";

void ThrowVideoInitializeException(const String& method)
{
	throw VideoInitializeException(method, SVideoFailed);
}

}
}
