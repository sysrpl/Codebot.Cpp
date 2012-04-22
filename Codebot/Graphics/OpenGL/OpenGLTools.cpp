#include <Codebot/Graphics/OpenGL/OpenGLTools.h>

namespace Codebot
{
namespace Graphics
{
namespace OpenGL
{

void glxClearColor(LongWord color)
{
	FloatColor f = color;
	glClearColor(f.R, f.G, f.B, f.A);
}

void glxClearColor(const FloatColor& color)
{
	glClearColor(color.R, color.G, color.B, color.A);
}

void glxColor(LongWord color)
{
	FloatColor f = color;
	glColor4f(f.R, f.G, f.B, f.A);
}

void glxColor(const FloatColor& color)
{
	glColor4f(color.R, color.G, color.B, color.A);
}

}
}
}
