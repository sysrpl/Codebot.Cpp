#pragma once
#include "Codebot/Array.h"

namespace Codebot
{

// String class

class String : public ValueType, public IArray<Char, Integer>, public ISerializable
{
private:
	Shared<std::string> data;
	void Unshare();
public:
	static const String Empty;
	static const String NewLine;
	String();
	String(const std::string& s);
	String(const String& s);
	String(const Char& c);
	String(const PChar s);
	operator const PChar () const;
	String operator + (const Char& c) const;
	String operator + (const String& s) const;
	String& operator += (const Char& c);
	String& operator += (const String& s);
	String& operator = (const String& s);
	Boolean operator == (const PChar s) const;
	Boolean operator == (const String& s) const;
	Boolean operator != (const PChar s) const;
	Boolean operator != (const String& s) const;
	Boolean operator < (const String& s) const;
	Boolean operator <= (const String& s) const;
	Boolean operator > (const String& s) const;
	Boolean operator >= (const String& s) const;
	String& Clear();
	String Append(const String& s) const;
	Integer Compare(const String& s) const;
	Boolean Contains(const String& s) const;
	Boolean ContainsAny(const String& s) const;
	Boolean BeginsWith(const String& s) const;
	Boolean EndsWith(const String& s) const;
	String FirstOf(const String& s) const;
	String SecondOf(const String& s) const;
	String LastOf(const String& s) const;
	Integer FirstIndexOf(const String& s) const;
	Integer LastIndexOf(const String& s) const;
	Integer IndexOf(const Char& c, Integer offset = 0) const;
	Integer IndexOf(const String& s, Integer offset = 0) const;
	String Insert(const String& s, Integer index) const;
	String Replace(const String& oldValue, const String& newValue) const;
	String ReplaceOne(const String& oldValue, const String& newValue, Integer index = 0) const;
	Array<String> Split(const String& delimiter) const;
	Array<String> SplitOne(const String& delimiter) const;
	String SubString(Integer start) const;
	String SubString(Integer start, Integer length) const;
	String ToUpper() const;
	String ToLower() const;
	String Pad(const Char& c, Integer count) const;
	String PadLeft(const Char& c, Integer count) const;
	String PadRight(const Char& c, Integer count) const;
	String Trim() const;
	String TrimLeft() const;
	String TrimRight() const;
	// Override methods
	Boolean Equals(ValueType* instance) const;
	// Properties
	Boolean IsBlank() const;
	Boolean IsEmpty() const;
	Cardinal Length() const;
	void Length(Cardinal value);
	// IArray<Char, Integer>
	Char& Item(const Integer& index);
	const Char& Item(const Integer& index) const;
	Char& operator [] (const Integer& index);
	const Char& operator [] (const Integer& index) const;
	// ISerializable
	String ToFormat(const String& format) const;
	Boolean FromFormat(const String& value, const String& format);

};

// Global string operators

Boolean operator == (const PChar c, const String& s);
Boolean operator < (const String& s, const PChar c);
Boolean operator < (const PChar c, const String& s);
Boolean operator <= (const String& s, const PChar c);
Boolean operator <= (const PChar c, const String& s);
Boolean operator > (const String& s, const PChar c);
Boolean operator > (const PChar c, const String& s);
Boolean operator >= (const String& s, const PChar c);
Boolean operator >= (const PChar c, const String& s);
String operator + (const PChar c, const String& s);
String operator + (const Char& c, const String& s);

// String related routines

template <typename T>
String TypeToStr(const T& value)
{
	return typeid(value).name();
}

namespace StringSpace
{
    template<typename T, typename CharT, typename Traits>
    std::basic_ostream<CharT, Traits>&
    operator << (std::basic_ostream<CharT, Traits>& stream, const T& item)
    {
    	String s = "{" + TypeToStr(item) + "}";
        return stream << s;
    }
}

template <typename T>
String AnyToStr(const T& value)
{
	using namespace StringSpace;
	std::string s;
	std::stringstream out;
	out << value;
	return String((const PChar)out.str().c_str());
}

Integer StrToInt(const String& s, Integer defaultValue = 0);
Double StrToDouble(const String& s, Double defaultValue = 0);
String StrJoin(const Array<String>& items, const String& delimiter);

Boolean CharInSet(const Char& c, const Char& a, const Char& b);
Boolean IsAlpha(const Char& c);
Boolean IsNumeric(const Char& c);
Boolean IsAlphaNumeric(const Char& c);

}
