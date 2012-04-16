#pragma once
#include "Codebot/Text/Format.h"

namespace Codebot
{

Char ReadChar();

String ReadLine(const String& defaultValue = String::Empty);

void Write(const String& s);

template <typename A0>
void Write(const String& s, const A0& a0)
{
	Write(Text::Format(s, a0));
}

template <typename A0, typename A1>
void Write(const String& s, const A0& a0, const A1& a1)
{
	Write(Text::Format(s, a0, a1));
}

template <typename A0, typename A1, typename A2>
void Write(const String& s, const A0& a0, const A1& a1, const A2& a2)
{
	Write(Text::Format(s, a0, a1, a2));
}

template <typename A0, typename A1, typename A2, typename A3>
void Write(const String& s, const A0& a0, const A1& a1, const A2& a2, 
	const A3& a3)
{
	Write(Text::Format(s, a0, a1, a2, a3));
}

template <typename A0, typename A1, typename A2, typename A3, typename A4>
void Write(const String& s, const A0& a0, const A1& a1, const A2& a2, 
	const A3& a3, const A4& a4)
{
	Write(Text::Format(s, a0, a1, a2, a3, a4));
}


void WriteLine(const String& s);

template <typename A0>
void WriteLine(const String& s, const A0& a0)
{
	WriteLine(Text::Format(s, a0));
}

template <typename A0, typename A1>
void WriteLine(const String& s, const A0& a0, const A1& a1)
{
	WriteLine(Text::Format(s, a0, a1));
}

template <typename A0, typename A1, typename A2>
void WriteLine(const String& s, const A0& a0, const A1& a1, const A2& a2)
{
	WriteLine(Text::Format(s, a0, a1, a2));
}

template <typename A0, typename A1, typename A2, typename A3>
void WriteLine(const String& s, const A0& a0, const A1& a1, const A2& a2, 
	const A3& a3)
{
	WriteLine(Text::Format(s, a0, a1, a2, a3));
}

template <typename A0, typename A1, typename A2, typename A3, typename A4>
void WriteLine(const String& s, const A0& a0, const A1& a1, const A2& a2, 
	const A3& a3, const A4& a4)
{
	WriteLine(Text::Format(s, a0, a1, a2, a3, a4));
}

}
