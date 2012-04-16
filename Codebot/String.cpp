#include "Codebot/String.h"
#include "Codebot/Exception.h"

namespace Codebot
{

const String String::Empty = "";
const String String::NewLine = "\n";

String::String()
{
	data = Empty.data;
}

String::String(const std::string& s)
{
	data = MakeShared<std::string>(s);
}

String::String(const String& s)
{
	data = s.data;
}

String::String(const Char& c)
{
	data = MakeShared<std::string>();
	data->push_back(c);
}

String::String(const PChar s)
{
	data = MakeShared<std::string>(s);
}

String::operator const PChar () const
{
	return data->c_str();
} 

String& String::operator = (const String& s)
{
	data = s.data;
	return *this;
}

String String::operator + (const String& s) const
{
	String r = (const PChar)*this;
	r.data->append(*s.data);
	return r;
}

String String::operator + (const Char& c) const
{
	String r = (const PChar)*this;
	r.data->push_back(c);
	return r;
}

String& String::operator += (const Char& c)
{
	Unshare();
	data->append(1, c);
	return *this;
}

String& String::operator += (const String& s)
{
	Unshare();
	data->append(*s.data);
	return *this;
}

Boolean String::operator == (const PChar s) const
{
	return data->compare(s) == 0;
}

Boolean String::operator == (const String& s) const
{
	return data->compare(*s.data) == 0;
}

Boolean String::operator != (const PChar s) const
{
	return data->compare(s) != 0;
}

Boolean String::operator != (const String& s) const
{
	return data->compare(*s.data) != 0;
}

Boolean String::operator < (const String& s) const
{
	return data->compare(*s.data) < 0;
}

Boolean String::operator <= (const String& s) const
{
	return data->compare(*s.data) < 1;
}

Boolean String::operator > (const String& s) const
{
	return data->compare(*s.data) > 0;
}

Boolean String::operator >= (const String& s) const
{
	return data->compare(*s.data) > -1;
}

void String::Unshare()
{
	if (!data.unique())
		data = MakeShared<std::string>(*data);
}

String& String::Clear()
{
	data = Empty.data;
	return *this;
}

String String::Append(const String& s) const
{
	return *this + s;
}

Integer String::Compare(const String& s) const
{
	return data->compare(s);
}

Boolean String::Contains(const String& s) const
{
	return data->find(s) != std::string::npos;
}

Boolean String::ContainsAny(const String& s) const
{
	return data->find_first_of(s) != std::string::npos;
}

Boolean String::BeginsWith(const String& s) const
{
	auto i = s.data->length();
	if (data->length() >= i) 
		return data->compare(0, i, *s.data) == 0;
	else
		return false;
}

Boolean String::EndsWith(const String& s) const
{
	auto i = s.data->length();
	if (data->length() >= i) 
		return data->compare(data->length() - i, i, *s.data) == 0;
	else
		return false;
}

String String::FirstOf(const String& s) const
{
	Integer i = data->find(s);
	if (i < 0)
		return String::Empty;
	return data->substr(0, i).c_str();
}

String String::SecondOf(const String& s) const
{
	Integer i = data->find(s);
	if (i < 0)
		return String::Empty;
	return data->substr(i  + s.data->size()).c_str();
}

String String::LastOf(const String& s) const
{
	Integer i = data->rfind(s);
	if (i < 0)
		return String::Empty;
	return data->substr(i  + s.data->length()).c_str();
}

Integer String::FirstIndexOf(const String& s) const
{
	return data->find(s);
}

Integer String::LastIndexOf(const String& s) const
{
	return data->rfind(s);
}

Integer String::IndexOf(const Char& c, Integer offset) const
{
	return data->find(c, offset);
}

Integer String::IndexOf(const String& s, Integer offset) const
{
	return data->find(s, offset);
}

String String::Insert(const String& s, Integer index) const
{
	if (index > (Integer)data->length())
		ThrowRangeException(ThisMethod);
	String r = (const PChar)*this;
	r.data->insert(index, s);
	return r;
}

String String::Replace(const String& oldValue, const String& newValue) const
{
	String r = (const PChar)*this;
	Integer i = 0;
	while ((i = r.data->find(*oldValue.data, i)) > -1)
	{
		r.data->replace(i, oldValue.data->length(), *newValue.data);
		i += newValue.data->length();
	}
	return r;
}

String String::ReplaceOne(const String& oldValue, const String& newValue, Integer index) const
{
	String r = (const PChar)*this;
	index = r.data->find(*oldValue.data, index);
	if (index > -1)
		r.data->replace(index, oldValue.data->length(), *newValue.data);
	return r;
}

Array<String> String::Split(const String& delimiter) const
{
	Array<String> r;
	std::string& a = *data;
	std::string& b = *delimiter.data;
	if (a.empty() || b.empty())
	{
		r[0] = *this;
		return r;
	}
	Integer alen = a.size();
	Integer blen = b.size();
	Integer head = 0;
	Integer tail = a.find(b, head);
	if (tail < 0)
	{
		r[0] = *this;
		return r;
	}
	Integer i = 0;
    while (tail > -1)
    {
		r[i++] = a.substr(head, tail - head).c_str();
		head = tail + blen;
        tail = a.find(b, head);
    }
	r[i++] = a.substr(head, alen - head).c_str();
	return r;
}

Array<String> String::SplitOne(const String& delimiter) const
{
	Array<String> r;
	Integer i = IndexOf(delimiter);
	if (i > -1)
	{
		r.Add(SubString(0, i));
		r.Add(SubString(i + delimiter.data->length()));
	}
	else
		r.Add(*this);
	return r;
}

String String::SubString(Integer start) const
{
	if (start < 0)
		ThrowRangeException(ThisMethod);
	if (start > (Integer)data->length())
		ThrowRangeException(ThisMethod);
	return data->substr(start).c_str();
}

String String::SubString(Integer start, Integer length) const
{
	if (start < 0)
		ThrowRangeException(ThisMethod);
	if (start > (Integer)data->length())
		ThrowRangeException(ThisMethod);
	return data->substr(start, length);
}

String String::ToUpper() const
{
	String r = (const PChar)*this;
	std::transform(r.data->begin(), r.data->end(), r.data->begin(), ::toupper);
	return r;
}

String String::ToLower() const
{
	String r = (const PChar)*this;
	std::transform(r.data->begin(), r.data->end(), r.data->begin(), ::tolower);
	return r;
}

String String::Pad(const Char& c, Integer count) const
{
	if (count == 0)
		return *this;
	return count < 0 ? PadLeft(c, -count) : PadRight(c, count);
}

String String::PadLeft(const Char& c, Integer count) const
{
	if (count < 1)
		return *this;
	Integer length = data->size();
	Integer i = count - length;
	if (i < 1)
		return *this;
	std::string buffer;
	buffer.reserve(i + length);
	while (i > 0)
	{
		buffer.push_back(c);
		i--;
	}
	buffer.append(*data);
	return (PChar)buffer.c_str();
}

String String::PadRight(const Char& c, Integer count) const
{
	if (count < 1)
		return *this;
	Integer length = data->size();
	Integer i = count - length;
	if (i < 1)
		return *this;
	std::string buffer;
	buffer.reserve(i + length);
	buffer.append(*data);
	while (i > 0)
	{
		buffer.push_back(c);
		i--;
	}
	return (PChar)buffer.c_str();
}

const std::string WhiteSpaces(" \f\n\r\t\v");

String String::Trim() const
{
	String r = (const PChar)*this;
	r.data->erase(0, r.data->find_first_not_of(WhiteSpaces));
	r.data->erase(r.data->find_last_not_of(WhiteSpaces) + 1);
    return r;
}

String String::TrimLeft() const
{
	String r = (const PChar)*this;
	r.data->erase(0, r.data->find_first_not_of(WhiteSpaces));
    return r;
}

String String::TrimRight() const
{
	String r = (const PChar)*this;
	r.data->erase(r.data->find_last_not_of(WhiteSpaces) + 1);
    return r;
}

Boolean String::IsBlank() const
{
	return Trim().data->empty();
}

Boolean String::IsEmpty() const
{
	return data->empty();
}

// String override methods

Boolean String::Equals(ValueType* instance) const
{
	return ValuesEqual(this, instance);
}

/* String::IArray<Char, Integer> */

Char& String::Item(const Integer& index)
{
	return (*this)[index];
}

const Char& String::Item(const Integer& index) const
{
	return (*this)[index];
}

Char& String::operator [] (const Integer& index)
{
	Unshare();
	return (*data)[index];
}

const Char& String::operator [] (const Integer& index) const
{
	return (*data)[index];
}

Cardinal String::Length() const
{
	return data->length();
}

void String::Length(Cardinal value) 
{
	if (value != data->length())
	{
		Unshare();
		if (value == 0)
			data = String::Empty.data;
		else
			data->resize(value);
	}
}

// String::ISerializable

String String::ToFormat(const String& format) const
{
	if (format.IsEmpty())
		return *this;
	if (format == "upper")
		return (*this).ToUpper();
	if (format == "lower")
		return (*this).ToUpper();
	if (format == "trim")
		return (*this).Trim();
	return *this;
}	

Boolean String::FromFormat(const String& value, const String& format)
{
	*this = value;
	return true;
}

/* Global string operators*/

Boolean operator == (const PChar c, const String& s)
{
	return String(c).Compare(s) == 0;
}

Boolean operator < (const String& s, const PChar c)
{
	return s < String(c);
}

Boolean operator < (const PChar c, const String& s)
{
	return String(c) < s;
}

Boolean operator <= (const String& s, const PChar c)
{
	return s <= String(c);
}

Boolean operator <= (const PChar c, const String& s)
{
	return String(c) <= s;
}

Boolean operator > (const String& s, const PChar c)
{
	return s > String(c);
}

Boolean operator > (const PChar c, const String& s)
{
	return String(c) > s;
}

Boolean operator >= (const String& s, const PChar c)
{
	return s >= String(c);
}

Boolean operator >= (const PChar c, const String& s)
{
	return String(c) >= s;
}

String operator + (const PChar c, const String& s)
{
	return String(c).Append(s);
}

String operator + (const Char& c, const String& s)
{
	return String(c).Append(s);
}

/* String related routines */

Integer StrToInt(const String& s, Integer defaultValue)
{
	Integer result = 0;
	Boolean negative = false;
	Integer length = s.Length();
	for (Integer i = 0; i < length; i++)
	{
		const Char c = s[i];
		if (i == 0 && c == '-')
		{
			negative = true;
			continue;
		}
		else  if (c >= '0' && c <= '9')
		{
			result *= 10;
			result += (c - '0');
		}
		else
			return defaultValue;
	}
	return (negative) ? -result : result;
}

Double StrToDouble(const String& s, Double defaultValue)
{
	Double result = 0;
	Boolean negative = false;
	Boolean point = false;
	Integer decimal = 1;
	Integer length = s.Length();
	for (Integer i = 0; i < length; i++)
	{
		const Char c = s[i];
		if (i == 0 && c == '-')
		{
			negative = true;
			continue;
		}
		if (c == '.')
		{
			if (point)
				return defaultValue;
			point = true;
			continue;
		}
		if (c >= '0' && c <= '9')
		{
			result *= 10;
			result += (c - '0');
			if (point)
				decimal *= 10;
		}
		else
			return defaultValue;
	}
	result = result / decimal;
	return (negative) ? -result : result;
}

String StrJoin(const Array<String>& items, const String& delimiter)
{
	auto length = items.Length();
	if (items.Length() == 0)
		return String::Empty;
	String s = items[0];
	for (Cardinal i = 1; i < length; i++)
	{
		s += delimiter;
		s += items[i];
	}
	return s;
}

inline Boolean CharInSet(const Char& c, const Char& a, const Char& b)
{
	return (c >= a) && (c <= b);
}

Boolean IsAlpha(const Char& c)
{
	return CharInSet(c, 'A', 'Z') || CharInSet(c, 'a', 'z') || c == '_';
}

Boolean IsNumeric(const Char& c)
{
	return CharInSet(c, '0', '9');
}

Boolean IsAlphaNumeric(const Char& c)
{
	return IsAlpha(c) || IsNumeric(c);
}

}
