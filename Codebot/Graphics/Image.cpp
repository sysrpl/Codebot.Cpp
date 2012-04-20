#include <Codebot/Graphics/Image.h>
#include <Codebot/Graphics/GraphicsExceptions.h>
#include <Codebot/Interop/IStream.h>
#include <Codebot/Text/Format.h>

namespace Codebot
{
namespace Graphics
{

using namespace Codebot::Interop;
using namespace Codebot::Graphics::Interop;

namespace ImageFormats
{
	const String jpg = "jpg";
	const String bmp = "bmp";
	const String gif = "gif";
	const String png = "png";
}

Image::Image() :
	image(null), format(ImageFormats::png)
{
	ImageCreate(Out(image));
}

void Image::Generate(Cardinal width, Cardinal height)
{
	if (!image->Generate(width, height))
		ThrowImageException(ThisMethod);
}

void Image::Flip()
{
	if (!image->Flip())
		ThrowImageException(ThisMethod);
}

void Image::Mirror()
{
	if (!image->Mirror())
		ThrowImageException(ThisMethod);
}

void Image::Resize(Cardinal width, Cardinal height)
{
	if (!image->Resize(width, height))
		ThrowImageException(ThisMethod);
}

// Image override methods

String Image::ToFormat(const String& format) const
{
	if (Loaded())
		return Text::Format("image {1}x{2}, format: {0}", this->format,
			image->Width(), image->Height());
	else
		return "empty image";
}


// Image properties

String Image::Format()
{
	return format;
}

void Image::Format(const String& value)
{
	String s = value.ToLower().Trim();
	if (s == ImageFormats::jpg)
		format = s;
	else if (s == ImageFormats::gif)
		format = s;
	else if (s == ImageFormats::bmp)
		format = s;
	else
		format = ImageFormats::png;
}

Boolean Image::Loaded() const
{
	return image->Loaded();
}

Cardinal Image::Width() const
{
	return image->Width();
}

Cardinal Image::Height() const
{
	return image->Height();
}

Pixel* Image::Pixels() const
{
	return (Pixel*)image->Bits();
}

// Image::IPersistent

void Image::Load(const String& fileName)
{
	format = image->GetFileFormat(fileName);
	if (!image->LoadFile(fileName))
		ThrowImageException(ThisMethod);
}

void Image::Load(Stream* stream)
{
	Interface<IStream> s = new StreamAdapter(stream);
	format = image->GetStreamFormat(s);
	if (!image->LoadStream(s))
		ThrowImageException(ThisMethod);
}

void Image::Save(const String& fileName) const
{
	if (!image->SaveFile(fileName))
		ThrowImageException(ThisMethod);
}

void Image::Save(Stream* stream) const
{
	auto s = QueryAs<IStream>(new StreamAdapter(stream));
	if (!image->SaveStream(format, s))
		ThrowImageException(ThisMethod);
}

}
}
