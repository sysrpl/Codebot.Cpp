#include <Codebot/String.h>
#include <Codebot/Text/Format.h>

namespace Codebot
{
namespace Text
{

FormatSection::FormatSection()
	: Argument(-1), Text(String::Empty), Padding(0)
{
}

Array<FormatSection> ParseFormat(const String& s)
{
	Array<FormatSection> result;
	FormatSection section;
	String argument;
	String text;
	String padding;
	Boolean escape = false;
	Boolean curly = false;
	Boolean colon = false;
	Boolean pipe = false;
	Integer length = s.Length();
	for (Integer i = 0; i < length; i++)
	{
		const Char c = s[i];
		if (escape)
		{
			text += c;
			continue;
		}
		else if (c == '\\')
		{
			escape = true;
			continue;
		}
		if (curly)
		{
			if (c == '}')
			{
				curly = false;
				colon = false;
				pipe = false;
				section.Argument = StrToInt(argument, -1);
				if (section.Argument < 0)
					section.Argument = -1;
				argument.Clear();
				section.Text = text;
				text.Clear();
				section.Padding = padding.IsEmpty() ? 0 : StrToInt(padding, 0);
				padding.Clear();
				result.Add(section);
				section = FormatSection();
			}
			else if (!colon && (c == ':'))
				colon = true;
			else if (!pipe && (c == '|'))
			{
				colon = true;
				pipe = true;
			}
			else if (pipe)
				padding += c;
			else if (colon)
				text += c;
			else
				argument += c;
		}
		else if (c == '{')
		{
			curly = true;
			if (!text.IsEmpty())
			{
				section.Text = text;
				result.Add(section);
				section = FormatSection();
			}
			argument.Clear();
			text.Clear();
			padding.Clear();
		}
		else
			text += c;
	}
	if (!text.IsEmpty())
	{
		section.Text = text;
		result.Add(section);
	}
	return result;
}

String FormatValue(const char value[], const String& format)
{
	String s = AnyToStr(value);
	return s.ToFormat(format);
}

String FormatValue(const Integer& value, const String& format)
{
	String prefix;
	String suffix;
	Integer count = 0;
	Integer length = format.Length();
	for (Integer i = 0; i < length; i++)
	{
		const Char c = format[i];
		if (c == '#')
			count++;
		else if (c == '|')
			break;
		else if (count == 0)
			prefix += c;
		else
			suffix += c;
	}
	String s = AnyToStr(Abs(value));
	s = s.PadLeft('0', count);
	if (value < 0)
		prefix += "-";
	return prefix + s + suffix;
}

String FormatValue(const Single& value, const String& format)
{
	double d = value;
	return FormatValue(d, format);
}

String FormatValue(const Double& value, const String& format)
{
	String prefix;
	String suffix;
	Integer left = 0;
	Integer right = 0;
	Boolean dot = false;
	Integer length = format.Length();
	for (Integer i = 0; i < length; i++)
	{
		const Char c = format[i];
		if (c == '#')
		{
			if (dot)
				right++;
			else
				left++;
		}
		else if (c == '.')
			dot = true;
		else if (c == '|')
			break;
		else if (dot)
			suffix += c;
		else
			prefix += c;
	}
	Double v = Abs(value);
	if (v < 0.00001)
		v = 0;
	if (left == 0 && right == 0)
		return prefix + AnyToStr(v) + suffix;
	String s = AnyToStr(v);
	String n = value < 0 ? "-" : String::Empty;
	auto pair = s.Split(".");
	if (pair.Length() < 2)
		pair.Add(String::Empty);
	if (left < 1)
		left = 1;
	pair[0] = pair[0].PadLeft('0', left);
	if (right == 0)
		return prefix + n + pair[0] + suffix;
	pair[1] = pair[1].PadRight('0', right);
	return prefix + n + pair[0] + String(".") + 
		pair[1].SubString(0, right) + suffix;
}

String FormatValue(const Boolean& value, const String& format)
{
	Integer length = format.Length();
	for (Integer i = 0; i < length; i++)
	{
		const Char c = format[i];
		switch (c)
		{
			case 'Y':
			case 'N':
				return value ? "Yes" : "No";
			case 'y':
			case 'n':
				return value ? "yes" : "no";
			case 'T':
			case 'F':
				return value ? "True" : "False";
			case 'P':
				return value ? "Pass" : "Fail";
			case 'p':
				return value ? "pass" : "fail";
			default:
				return value ? "true" : "false";
		}
	}
	return value ? "true" : "false";
}

String FormatValue(const ValueType& value, const String& format)
{
	return value.ToFormat(format);
}

String FormatValue(const ValueType* value, const String& format) 
{ 
	if (value == null)  
		return "(null)"; return value->ToFormat(format);
}

}
}
