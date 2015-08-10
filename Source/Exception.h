#pragma once
#include <Codebot/Text/Format.h>

namespace Codebot
{

class Exception : public ValueType
{
private:
	String a;
	String b;
public:
	Exception(const String& method, const String& message);
	String ToFormat(const String& format) const;
	const String Method() const;
	const String Message() const;
};

class AbortException : public Exception
{
public:
	AbortException(const String& method, const String& message)
		: Exception(method, message) { }
};

class CastException : public Exception
{
public:
	CastException(const String& method, const String& message)
		: Exception(method, message) { }
};

class CopyObjectException : public Exception
{
public:
	CopyObjectException(const String& method, const String& message)
		: Exception(method, message) { }
};

class AssignException : public Exception
{
public:
	AssignException(const String& method, const String& message)
		: Exception(method, message) { }
};


class ArgumentException : public Exception
{
public:
	ArgumentException(const String& method, const String& message)
		: Exception(method, message) { }
};

class NullReferenceException : public Exception
{
public:
	NullReferenceException(const String& method, const String& message)
		: Exception(method, message) { }
};

class RangeException : public Exception
{
public:
	RangeException(const String& method, const String& message)
		: Exception(method, message) { }
};

class SystemException : public Exception
{
private:
	Integer c;
public:
	SystemException(const String& method, const String& message, Integer code)
		: Exception(method, message), c(code) { }
	const Integer Code() const;
};

}
