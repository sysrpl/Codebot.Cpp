#pragma once
#include <Codebot/Graphics/GraphicsTypes.h>
#include <Codebot/Graphics/Interop/GraphicsInterop.h>
#include <Codebot/Interop/Interface.h>
#include <Codebot/Stream.h>
#include <Codebot/IPersistent.h>

namespace Codebot
{
namespace Graphics
{

namespace ImageFileTypes
{
	extern const String bmp;
	extern const String gif;
	extern const String jpg;
	extern const String png;
	extern const String tga;
}

typedef Codebot::Graphics::Interop::IImage IImage;

// Image class

class Image : public ValueType, public IPersistent
{
private:
	Codebot::Interop::Interface<IImage> image;
	String fileType;
public:
	Image();
	void Generate(Cardinal width, Cardinal height);
	void Flip();
	void Mirror();
	void Resize(Cardinal width, Cardinal height);
	void Premultiply();
	// Override methods
	String ToFormat(const String& format) const;
	// Properties
	String FileType();
	void FileType(const String& value);
	Boolean Loaded() const;
	Cardinal Width() const;
	Cardinal Height() const;
	Pixel* Pixels() const;
	// IPersistent
	void Load(const String& fileName);
	void Load(Stream* stream);
	void Save(const String& fileName) const;
	void Save(Stream* stream) const;
};

}
}
