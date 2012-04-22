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

namespace ImageFileTypes
{
	const String bmp = "bmp";
	const String gif = "gif";
	const String jpg = "jpg";
	const String png = "png";
	const String tga = "tga";
}

Image::Image() :
	image(null), fileType(ImageFileTypes::png)
{
	ImageCreate(Out(image));
}

void Image::Generate(Cardinal width, Cardinal height)
{
	if (width < 1)
		ThrowArgumentException(ThisMethod, "width");
	if (height < 1)
		ThrowArgumentException(ThisMethod, "height");
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

Boolean NeedsPremultiply(Pixel* pixel, Cardinal width, Cardinal height)
{
	auto size = width * height;
	for (Cardinal i = 0; i < size; i++)
	{
		auto a = pixel[i].A;
		if (a == 0xFF)
			continue;
		if (pixel[i].R > a)
			return true;
		if (pixel[i].G > a)
			return true;
		if (pixel[i].B > a)
			return true;
	}
	return false;
}

void Image::Premultiply()
{
	if (!Loaded())
		return;
	Pixel* pixel = Pixels();
	auto width = Width();
	auto height = Height();
	if (!NeedsPremultiply(pixel, width, height))
		return;
	auto size = width * height;
	for (Cardinal i = 0; i < size; i++)
	{
		auto a = pixel[i].A;
		if (a == 0xFF)
			continue;
		if (a == 0)
		{
			pixel[i].R = 0;
			pixel[i].G = 0;
			pixel[i].B = 0;
			continue;
		}
		Single ratio = (Single)pixel[i].A / 255.0f;
		pixel[i].R = (Byte)(pixel[i].R * ratio);
		pixel[i].G = (Byte)(pixel[i].G * ratio);
		pixel[i].B = (Byte)(pixel[i].B * ratio);
	}
}

// Image override methods

String Image::ToFormat(const String& format) const
{
	if (Loaded())
		return Text::Format("image {1}x{2}, format: {0}", fileType,
			image->Width(), image->Height());
	else
		return "empty image";
}

// Image properties

String Image::FileType()
{
	return fileType;
}

void Image::FileType(const String& value)
{
	String s = value.ToLower().Trim();
	if (s == ImageFileTypes::bmp)
		fileType = s;
	else if (s == ImageFileTypes::gif)
		fileType = s;
	else if (s == ImageFileTypes::jpg)
		fileType = s;
	else if (s == ImageFileTypes::tga)
		fileType = s;
	else
		fileType = ImageFileTypes::png;
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
	Pixel* pixels = (Pixel*)image->Bits();
	if (pixels == null)
		ThrowNullReferenceException(ThisMethod, "pixels");
	return (Pixel*)image->Bits();
}

// Image::IPersistent

void Image::Load(const String& fileName)
{
	fileType = image->GetFileFormat(fileName);
	if (!image->LoadFile(fileName))
		ThrowImageException(ThisMethod);
}

void Image::Load(Stream* stream)
{
	Interface<IStream> s = new StreamAdapter(stream);
	fileType = image->GetStreamFormat(s);
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
	if (!image->SaveStream(fileType, s))
		ThrowImageException(ThisMethod);
}

}
}
