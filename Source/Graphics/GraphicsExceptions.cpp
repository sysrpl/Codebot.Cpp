#include <Codebot/Graphics/GraphicsExceptions.h>

namespace Codebot
{
namespace Graphics
{

const String SImageFailed = "An image method failed";

void ThrowImageException(const String& method)
{
	throw ImageException(method, SImageFailed);
}

}
}
