#pragma once
#include <Codebot/ValueType.h>
#include <Codebot/Interop/DateInterop.h>

namespace Codebot
{
namespace Time
{

class DateTime;

// TimeSpan type

class TimeSpan : public ValueType, public ISerializable
{
private:
	Interop::Time data;
public:
	friend class DateTime;
	static const TimeSpan Empty;
	static TimeSpan Now();
	TimeSpan();
	TimeSpan(Integer hour, Integer minute = 0, Integer second = 0, Integer millisecond = 0);
	TimeSpan(const DateTime& dateTime);
	TimeSpan(const TimeSpan& t);
	TimeSpan& operator = (const TimeSpan& t);
	TimeSpan& operator += (const TimeSpan& t);
	TimeSpan& operator -= (const TimeSpan& t);
	TimeSpan operator + (const TimeSpan& t) const;
	TimeSpan operator - (const TimeSpan& t) const;
	Boolean operator == (const TimeSpan& t) const;
	Boolean operator != (const TimeSpan& t) const;
	Boolean operator > (const TimeSpan& t) const;
	Boolean operator >= (const TimeSpan& t) const;
	Boolean operator < (const TimeSpan& t) const;
	Boolean operator <= (const TimeSpan& t) const;
	TimeSpan AddHours(Integer h) const;
	TimeSpan AddMinutes(Integer m) const;
	TimeSpan AddSeconds(Integer s) const;
	TimeSpan AddMilliseconds(Integer m) const;
	Integer Hours() const;
	Integer Minutes() const;
	Integer Seconds() const;
	Integer Milliseconds() const;
	//  ISeriaizable
	String ToFormat(const String& format) const;
	Boolean FromFormat(const String& value, const String& format);
};

}
}
