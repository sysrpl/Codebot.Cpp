#include <Codebot/Graphics/GraphicsTypes.h>
#include <Codebot/Collections/List.h>
#include <Codebot/Memory/Buffer.h>
#include <Codebot/Console.h>

namespace Codebot
{
namespace Graphics
{

using namespace Collections;
using namespace Memory;

// ARGB struct

ARGB::ARGB()
{
	A = R = G = B = 0;
}

ARGB::ARGB(LongWord argb)
{
	A = argb >> 24;
	R = (argb >> 16) & 0xFF;
	G = (argb >> 8) & 0xFF;
	B = argb & 0xFF;
}

ARGB::ARGB(Byte r, Byte g, Byte b)
{
	A = 0xFF;
	R = r;
	G = g;
	B = b;
}

ARGB::ARGB(Byte a, Byte r, Byte g, Byte b)
{
	A = a;
	R = r;
	G = g;
	B = b;
}

ARGB::operator LongWord () const
{
	return (A << 24) | (R << 16) | (G << 8) | B;
}

LongWord ARGB::operator ()() const
{
	return (A << 24) | (R << 16) | (G << 8) | B;
}

Boolean ARGB::operator ==(const ARGB& value) const
{
	return (A == value.A) && (R == value.R) && (G == value.G) && (B == value.B);
}

// Color class

Color::Color()
{
}

Color::Color(ARGB argb) :
	color(argb)
{
}

Color::Color(LongWord c) :
	color(c)
{
}

Color::Color(Byte r, Byte g, Byte b) :
	color(r, g, b)
{
}

Color::Color(Byte a, Byte r, Byte g, Byte b) :
	color(a, r, g, b)
{
}

Boolean Color::operator == (const Color& value) const
{
	return color == value.color;
}

Color::operator LongWord () const
{
	return color;
}

LongWord Color::operator ()() const
{
	return color;
}

Color Color::Blend(const Color& color, Float percent) const
{
	percent = Clamp(percent);
	Float inverse = 1.0f - percent;
	ARGB argb;
	const ARGB& a = color.color;
	const ARGB& b = this->color;
	argb.A = (Byte)(a.A * percent + b.A * inverse);
	argb.R = (Byte)(a.R * percent + b.R * inverse);
	argb.G = (Byte)(a.G * percent + b.G * inverse);
	argb.B = (Byte)(a.B * percent + b.B * inverse);
	return argb;
}

Color Color::Compliment() const
{
	return Color(color.A, 0xFF - color.R, 0xFF - color.G, 0xFF - color.B);
}

Color Color::Opacity(Float percent) const
{
	Color c = color;
	c.color.A = (Byte)(0xFF * Clamp(percent));
	return c;
}

// Override methods

Boolean Color::Equals(ValueType* instance) const
{
	return ValuesEqual(this, instance);
}

// Color::ISerializable

String Color::ToFormat(const String& format) const
{
	return ColorToStr(color);
}

Boolean Color::FromFormat(const String& value, const String& format)
{
	color = StrToColor(value);
	return true;
}

// FloatColor struct

#define ColorMix(a) a / 255.0f
#define ColorUnmix(a) Byte(Clamp(a) * 255.0f)

FloatColor::FloatColor()
{
	A = R = G = B = 0.0f;
}

FloatColor::FloatColor(ARGB argb) :
	B(ColorMix(argb.B)),
	G(ColorMix(argb.G)),
	R(ColorMix(argb.R)),
	A(ColorMix(argb.A))
{
}

FloatColor::FloatColor(LongWord color)
{
	ARGB argb(color);
	B = ColorMix(argb.B);
	G = ColorMix(argb.G);
	R = ColorMix(argb.R);
	A = ColorMix(argb.A);
}

FloatColor::FloatColor(Float r, Float g, Float b) :
	B(b),
	G(g),
	R(r),
	A(1.0f)
{
}

FloatColor::FloatColor(Float a, Float r, Float g, Float b) :
	B(b),
	G(g),
	R(r),
	A(a)
{
}

Boolean FloatColor::operator == (const FloatColor& value) const
{
	return (A == value.A) && (R == value.R) && (G == value.G) && (B == value.B);
}

FloatColor::operator LongWord () const
{
	return (ColorUnmix(A) << 24) | (ColorUnmix(R) << 16) | (ColorUnmix(G) << 8) | ColorUnmix(B);
}

LongWord FloatColor::operator ()() const
{
	return (ColorUnmix(A) << 24) | (ColorUnmix(R) << 16) | (ColorUnmix(G) << 8) | ColorUnmix(B);
}

// Color conversion routines

Ref<List<String>> ColorList;
Ref<List<LongWord>> PixelList;

void BuildColorList()
{
	ColorList = New<List<String>>();
	List<String>& c = ColorList.Value();
	c.Add("Transparent");
	c.Add("AliceBlue");
	c.Add("AntiqueWhite");
	c.Add("Aqua");
	c.Add("Aquamarine");
	c.Add("Azure");
	c.Add("Beige");
	c.Add("Bisque");
	c.Add("Black");
	c.Add("BlanchedAlmond");
	c.Add("Blue");
	c.Add("BlueViolet");
	c.Add("Brown");
	c.Add("BurlyWood");
	c.Add("CadetBlue");
	c.Add("Chartreuse");
	c.Add("Chocolate");
	c.Add("Coral");
	c.Add("CornflowerBlue");
	c.Add("Cornsilk");
	c.Add("Crimson");
	c.Add("Cyan");
	c.Add("DarkBlue");
	c.Add("DarkCyan");
	c.Add("DarkGoldenRod");
	c.Add("DarkGray");
	c.Add("DarkGrey");
	c.Add("DarkGreen");
	c.Add("DarkKhaki");
	c.Add("DarkMagenta");
	c.Add("DarkOliveGreen");
	c.Add("Darkorange");
	c.Add("DarkOrchid");
	c.Add("DarkRed");
	c.Add("DarkSalmon");
	c.Add("DarkSeaGreen");
	c.Add("DarkSlateBlue");
	c.Add("DarkSlateGray");
	c.Add("DarkSlateGrey");
	c.Add("DarkTurquoise");
	c.Add("DarkViolet");
	c.Add("DeepPink");
	c.Add("DeepSkyBlue");
	c.Add("DimGray");
	c.Add("DimGrey");
	c.Add("DodgerBlue");
	c.Add("FireBrick");
	c.Add("FloralWhite");
	c.Add("ForestGreen");
	c.Add("Fuchsia");
	c.Add("Gainsboro");
	c.Add("GhostWhite");
	c.Add("Gold");
	c.Add("GoldenRod");
	c.Add("Gray");
	c.Add("Grey");
	c.Add("Green");
	c.Add("GreenYellow");
	c.Add("HoneyDew");
	c.Add("HotPink");
	c.Add("IndianRed");
	c.Add("Indigo");
	c.Add("Ivory");
	c.Add("Khaki");
	c.Add("Lavender");
	c.Add("LavenderBlush");
	c.Add("LawnGreen");
	c.Add("LemonChiffon");
	c.Add("LightBlue");
	c.Add("LightCoral");
	c.Add("LightCyan");
	c.Add("LightGoldenRodYellow");
	c.Add("LightGray");
	c.Add("LightGrey");
	c.Add("LightGreen");
	c.Add("LightPink");
	c.Add("LightSalmon");
	c.Add("LightSeaGreen");
	c.Add("LightSkyBlue");
	c.Add("LightSlateGray");
	c.Add("LightSlateGrey");
	c.Add("LightSteelBlue");
	c.Add("LightYellow");
	c.Add("Lime");
	c.Add("LimeGreen");
	c.Add("Linen");
	c.Add("Magenta");
	c.Add("Maroon");
	c.Add("MediumAquaMarine");
	c.Add("MediumBlue");
	c.Add("MediumOrchid");
	c.Add("MediumPurple");
	c.Add("MediumSeaGreen");
	c.Add("MediumSlateBlue");
	c.Add("MediumSpringGreen");
	c.Add("MediumTurquoise");
	c.Add("MediumVioletRed");
	c.Add("MidnightBlue");
	c.Add("MintCream");
	c.Add("MistyRose");
	c.Add("Moccasin");
	c.Add("NavajoWhite");
	c.Add("Navy");
	c.Add("OldLace");
	c.Add("Olive");
	c.Add("OliveDrab");
	c.Add("Orange");
	c.Add("OrangeRed");
	c.Add("Orchid");
	c.Add("PaleGoldenRod");
	c.Add("PaleGreen");
	c.Add("PaleTurquoise");
	c.Add("PaleVioletRed");
	c.Add("PapayaWhip");
	c.Add("PeachPuff");
	c.Add("Peru");
	c.Add("Pink");
	c.Add("Plum");
	c.Add("PowderBlue");
	c.Add("Purple");
	c.Add("Red");
	c.Add("RosyBrown");
	c.Add("RoyalBlue");
	c.Add("SaddleBrown");
	c.Add("Salmon");
	c.Add("SandyBrown");
	c.Add("SeaGreen");
	c.Add("SeaShell");
	c.Add("Sienna");
	c.Add("Silver");
	c.Add("SkyBlue");
	c.Add("SlateBlue");
	c.Add("SlateGray");
	c.Add("SlateGrey");
	c.Add("Snow");
	c.Add("SpringGreen");
	c.Add("SteelBlue");
	c.Add("Tan");
	c.Add("Teal");
	c.Add("Thistle");
	c.Add("Tomato");
	c.Add("Turquoise");
	c.Add("Violet");
	c.Add("Wheat");
	c.Add("White");
	c.Add("WhiteSmoke");
	c.Add("Yellow");
	PixelList = New<List<LongWord>>();
	List<LongWord>& p = PixelList.Value();
	p.Add(Colors::Transparent);
	p.Add(Colors::AliceBlue);
	p.Add(Colors::AntiqueWhite);
	p.Add(Colors::Aqua);
	p.Add(Colors::Aquamarine);
	p.Add(Colors::Azure);
	p.Add(Colors::Beige);
	p.Add(Colors::Bisque);
	p.Add(Colors::Black);
	p.Add(Colors::BlanchedAlmond);
	p.Add(Colors::Blue);
	p.Add(Colors::BlueViolet);
	p.Add(Colors::Brown);
	p.Add(Colors::BurlyWood);
	p.Add(Colors::CadetBlue);
	p.Add(Colors::Chartreuse);
	p.Add(Colors::Chocolate);
	p.Add(Colors::Coral);
	p.Add(Colors::CornflowerBlue);
	p.Add(Colors::Cornsilk);
	p.Add(Colors::Crimson);
	p.Add(Colors::Cyan);
	p.Add(Colors::DarkBlue);
	p.Add(Colors::DarkCyan);
	p.Add(Colors::DarkGoldenRod);
	p.Add(Colors::DarkGray);
	p.Add(Colors::DarkGrey);
	p.Add(Colors::DarkGreen);
	p.Add(Colors::DarkKhaki);
	p.Add(Colors::DarkMagenta);
	p.Add(Colors::DarkOliveGreen);
	p.Add(Colors::Darkorange);
	p.Add(Colors::DarkOrchid);
	p.Add(Colors::DarkRed);
	p.Add(Colors::DarkSalmon);
	p.Add(Colors::DarkSeaGreen);
	p.Add(Colors::DarkSlateBlue);
	p.Add(Colors::DarkSlateGray);
	p.Add(Colors::DarkSlateGrey);
	p.Add(Colors::DarkTurquoise);
	p.Add(Colors::DarkViolet);
	p.Add(Colors::DeepPink);
	p.Add(Colors::DeepSkyBlue);
	p.Add(Colors::DimGray);
	p.Add(Colors::DimGrey);
	p.Add(Colors::DodgerBlue);
	p.Add(Colors::FireBrick);
	p.Add(Colors::FloralWhite);
	p.Add(Colors::ForestGreen);
	p.Add(Colors::Fuchsia);
	p.Add(Colors::Gainsboro);
	p.Add(Colors::GhostWhite);
	p.Add(Colors::Gold);
	p.Add(Colors::GoldenRod);
	p.Add(Colors::Gray);
	p.Add(Colors::Grey);
	p.Add(Colors::Green);
	p.Add(Colors::GreenYellow);
	p.Add(Colors::HoneyDew);
	p.Add(Colors::HotPink);
	p.Add(Colors::IndianRed);
	p.Add(Colors::Indigo);
	p.Add(Colors::Ivory);
	p.Add(Colors::Khaki);
	p.Add(Colors::Lavender);
	p.Add(Colors::LavenderBlush);
	p.Add(Colors::LawnGreen);
	p.Add(Colors::LemonChiffon);
	p.Add(Colors::LightBlue);
	p.Add(Colors::LightCoral);
	p.Add(Colors::LightCyan);
	p.Add(Colors::LightGoldenRodYellow);
	p.Add(Colors::LightGray);
	p.Add(Colors::LightGrey);
	p.Add(Colors::LightGreen);
	p.Add(Colors::LightPink);
	p.Add(Colors::LightSalmon);
	p.Add(Colors::LightSeaGreen);
	p.Add(Colors::LightSkyBlue);
	p.Add(Colors::LightSlateGray);
	p.Add(Colors::LightSlateGrey);
	p.Add(Colors::LightSteelBlue);
	p.Add(Colors::LightYellow);
	p.Add(Colors::Lime);
	p.Add(Colors::LimeGreen);
	p.Add(Colors::Linen);
	p.Add(Colors::Magenta);
	p.Add(Colors::Maroon);
	p.Add(Colors::MediumAquaMarine);
	p.Add(Colors::MediumBlue);
	p.Add(Colors::MediumOrchid);
	p.Add(Colors::MediumPurple);
	p.Add(Colors::MediumSeaGreen);
	p.Add(Colors::MediumSlateBlue);
	p.Add(Colors::MediumSpringGreen);
	p.Add(Colors::MediumTurquoise);
	p.Add(Colors::MediumVioletRed);
	p.Add(Colors::MidnightBlue);
	p.Add(Colors::MintCream);
	p.Add(Colors::MistyRose);
	p.Add(Colors::Moccasin);
	p.Add(Colors::NavajoWhite);
	p.Add(Colors::Navy);
	p.Add(Colors::OldLace);
	p.Add(Colors::Olive);
	p.Add(Colors::OliveDrab);
	p.Add(Colors::Orange);
	p.Add(Colors::OrangeRed);
	p.Add(Colors::Orchid);
	p.Add(Colors::PaleGoldenRod);
	p.Add(Colors::PaleGreen);
	p.Add(Colors::PaleTurquoise);
	p.Add(Colors::PaleVioletRed);
	p.Add(Colors::PapayaWhip);
	p.Add(Colors::PeachPuff);
	p.Add(Colors::Peru);
	p.Add(Colors::Pink);
	p.Add(Colors::Plum);
	p.Add(Colors::PowderBlue);
	p.Add(Colors::Purple);
	p.Add(Colors::Red);
	p.Add(Colors::RosyBrown);
	p.Add(Colors::RoyalBlue);
	p.Add(Colors::SaddleBrown);
	p.Add(Colors::Salmon);
	p.Add(Colors::SandyBrown);
	p.Add(Colors::SeaGreen);
	p.Add(Colors::SeaShell);
	p.Add(Colors::Sienna);
	p.Add(Colors::Silver);
	p.Add(Colors::SkyBlue);
	p.Add(Colors::SlateBlue);
	p.Add(Colors::SlateGray);
	p.Add(Colors::SlateGrey);
	p.Add(Colors::Snow);
	p.Add(Colors::SpringGreen);
	p.Add(Colors::SteelBlue);
	p.Add(Colors::Tan);
	p.Add(Colors::Teal);
	p.Add(Colors::Thistle);
	p.Add(Colors::Tomato);
	p.Add(Colors::Turquoise);
	p.Add(Colors::Violet);
	p.Add(Colors::Wheat);
	p.Add(Colors::White);
	p.Add(Colors::WhiteSmoke);
	p.Add(Colors::Yellow);
}

String ColorToStr(const LongWord& value)
{
	if (!ColorList.HasValue())
		BuildColorList();
	Integer i = PixelList->IndexOf(value);
	if (i < 0)
	{
		LongWord c = value;
		String s = Hex::Encode((Byte*)&c, sizeof(c)).Reverse();
		for (Integer j = 0; j < 4; j++)
			Swap(s[j * 2], s[j * 2 + 1]);
		return s;
	}
	return ColorList->Item(i);
}

LongWord StrToColor(const String& value)
{
	if (!ColorList.HasValue())
		BuildColorList();
	String s = value.Trim().ToUpper();
	Integer i = ColorList->Find([&s] (String a) { return s == a.ToUpper(); });
	if (i < 0)
	{
		if (IsOdd(s.Length()))
			return Colors::Transparent;
		if (s.Length() > 8)
			return Colors::Transparent;
		while (s.Length() < 6)
			s = "00" + s;
		if (s.Length() == 6)
			s = "FF" + s;
		try
		{
			ARGB a;
			auto b = Hex::Decode(s);
			if (b.Size() != sizeof(a))
				return Colors::Transparent;
			MemCopy(b, &a, sizeof(a));
			Swap(a.A, a.B);
			Swap(a.R, a.G);
			return a;
		}
		catch (ArgumentException& e)
		{
			return Colors::Transparent;
		}
	}
	return PixelList->Item(i);
}

}
}
