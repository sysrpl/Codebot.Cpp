#pragma once
#include "Codebot/Property.h"
#include "Codebot/String.h"

namespace Codebot
{
namespace Text
{

struct FormatSection
{
	FormatSection();
	Integer Argument;
	String Text;
	Integer Padding;
};

Array<FormatSection> ParseFormat(const String& s);

//  TODO: Consider FormatValue(more integral types)

String FormatValue(const char value[], const String& format);
String FormatValue(const Integer& value, const String& format);
String FormatValue(const Single& value, const String& format);
String FormatValue(const Double& value, const String& format);
String FormatValue(const Boolean& value, const String& format);
String FormatValue(const ValueType& value, const String& format);
String FormatValue(const ValueType* value, const String& format);

// TODO: Consider adding more type traits

template <typename T>
typename std::enable_if<std::is_base_of<ValueType, T>::value, String>::type
FormatValue(const T& value, const String& format) 
{ 
	return value.ToFormat(format);
}

template <typename T>
typename std::enable_if<!(std::is_base_of<ValueType, T>::value || 
	std::is_convertible<T, ValueType*>::value), String>::type
FormatValue(const T& value, const String& format) 
{ 
	return AnyToStr(value);
}

template <typename C, typename T, bool B>
String FormatValue(const Property<C, T, B>& prop, const String& format)
{
	PropType(prop) value = prop;
	return FormatValue(value, format);
}

template <typename A0, typename A1, typename A2, typename A3, typename A4>
String Format(const String& s, const A0& a0, const A1& a1, const A2& a2, 
	const A3& a3, const A4& a4)
{
	auto result = String::Empty;
	auto item = String::Empty;
	auto sections = ParseFormat(s);
	auto length = sections.Length();
	for (Cardinal i = 0; i < length; i++)
	{
		auto section = sections[i];
		if (section.Argument < 0)
		{
			result += section.Text;
			continue;
		}
		switch (section.Argument)
		{
			case 0:
				item = FormatValue(a0, section.Text);
				break;
			case 1:
				item = FormatValue(a1, section.Text);
				break;
			case 2:
				item = FormatValue(a2, section.Text);
				break;
			case 3:
				item = FormatValue(a3, section.Text);
				break;
			case 4:
				item = FormatValue(a4, section.Text);
				break;
			default:
				item.Clear();
				break;
		}
		if (section.Padding != 0)
			item = item.SubString(0, Abs(section.Padding));
		result += item.Pad(' ', section.Padding);
	}
	return result;
}

template <typename A0, typename A1, typename A2, typename A3>
String Format(const String& s, const A0& a0, const A1& a1, const A2& a2, 
	const A3& a3)
{
	const String& e = String::Empty;
	return Format(s, a0, a1, a2, a3, e);
}

template <typename A0, typename A1, typename A2>
String Format(const String& s, const A0& a0, const A1& a1, const A2& a2)
{
	const String& e = String::Empty;
	return Format(s, a0, a1, a2, e, e);
}

template <typename A0, typename A1>
String Format(const String& s, const A0& a0, const A1& a1)
{
	const String& e = String::Empty;
	return Format(s, a0, a1, e, e, e);
}

template <typename A0>
String Format(const String& s, const A0& a0)
{
	const String& e = String::Empty;
	return Format(s, a0, e, e, e, e);
}

}
}
