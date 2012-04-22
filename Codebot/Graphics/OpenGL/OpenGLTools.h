#pragma once
#include <Codebot/Packages/Graphics.h>
#include <Codebot/String.h>
#include <Codebot/Stream.h>
#include <GL/gl.h>
#include <GL/glu.h>

namespace Codebot
{
namespace Graphics
{
namespace OpenGL
{

/*
The following functions are extensions to OpenGL. These extensions
provide management and easy access to common OpenGL related tasks.

The extensions are all marked with the prefix glx.
*/

void glxClearColor(LongWord color);
void glxClearColor(const FloatColor& color);

void glxColor(LongWord color);
void glxColor(const FloatColor& color);

/*void glxInitialize();

void glxCaptureScreen(const String& fileName);
void glxCaptureScreen(Stream* stream);

void glxLoadTexture(const String& fileName);
void glxLoadTexture(Stream* stream);

void glxBeginOrtho(Cardinal width, Cardinal height);
void glxBeginEnd();*/

}
}
}
