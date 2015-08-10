#pragma once
#include <Codebot/Graphics/OpenGL/OpenGLTools.h>

namespace Codebot
{
namespace Graphics
{
namespace OpenGL
{

class TextWriter : public ValueType
{
public:
	TextWriter();
	void Write(const String& s);
	void WriteLine(const String& s);
};

}
}
}
