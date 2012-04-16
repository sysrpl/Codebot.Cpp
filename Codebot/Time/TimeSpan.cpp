#include "Codebot/Time/TimeSpan.h"
#include "Codebot/Time/DateTime.h"
#include "Codebot/Text/Format.h"

namespace Codebot
{
namespace Time
{

using namespace Interop;
using namespace Text;

const TimeSpan TimeSpan::Empty;

TimeSpan TimeSpan::Now()
{
	TimeSpan r;
	r.data = TimeNow();
	return r;
}

TimeSpan::TimeSpan()
	: data(0)
{
}

TimeSpan::TimeSpan(Integer hour, Integer minute, Integer second, Integer millisecond)
{
	TimeEncode(data, hour, minute, second, millisecond);
}

TimeSpan::TimeSpan(const TimeSpan& t)
{
	data = t.data;
}

TimeSpan::TimeSpan(const DateTime& dateTime)
	: data(Fract(dateTime.data))
{
}


TimeSpan& TimeSpan::operator = (const TimeSpan& t) 
{
	data = t.data;
	return *this;
}

TimeSpan& TimeSpan::operator += (const TimeSpan& t)
{
	data += t.data;
	return *this;
}

TimeSpan& TimeSpan::operator -= (const TimeSpan& t)
{
	data -= t.data;
	return *this;
}

TimeSpan TimeSpan::operator + (const TimeSpan& t) const
{
	TimeSpan r = *this;
	r.data += t.data;
	return r;
}

TimeSpan TimeSpan::operator - (const TimeSpan& t) const
{
	TimeSpan r = *this;
	r.data -= t.data;
	return r;
}

Boolean TimeSpan::operator == (const TimeSpan& t) const
{
	return data == t.data;
}

Boolean TimeSpan::operator != (const TimeSpan& t) const
{
	return data != t.data;
}

Boolean TimeSpan::operator > (const TimeSpan& t) const
{
	return data > t.data;
}

Boolean TimeSpan::operator >= (const TimeSpan& t) const
{
	return data >= t.data;
}

Boolean TimeSpan::operator < (const TimeSpan& t) const
{
	return data < t.data;
}

Boolean TimeSpan::operator <= (const TimeSpan& t) const
{
	return data <= t.data;
}

TimeSpan TimeSpan::AddHours(Integer h) const
{
	TimeSpan r = *this;
	r.data = TimeAddHour(r.data, h);
	return r;
}

TimeSpan TimeSpan::AddMinutes(Integer m) const
{
	TimeSpan r = *this;
	r.data = TimeAddMinute(r.data, m);
	return r;
}

TimeSpan TimeSpan::AddSeconds(Integer s) const
{
	TimeSpan r = *this;
	r.data = TimeAddSecond(r.data, s);
	return r;
}

TimeSpan TimeSpan::AddMilliseconds(Integer m) const
{
	TimeSpan r = *this;
	r.data = TimeAddMillisecond(r.data, m);
	return r;
}

Integer TimeSpan::Hours() const
{
	return TimeHours(data);
}

Integer TimeSpan::Minutes() const
{
	return TimeMinutes(data);
}

Integer TimeSpan::Seconds() const
{
	return TimeSeconds(data);
}

Integer TimeSpan::Milliseconds() const
{
	return TimeMilliseconds(data);
}

String TimeSpan::ToFormat(const String& format) const
{
	String s = format.IsEmpty() ? "hh:mm:ss" : format;
	s = s
		.Replace("hh", "{0:##}")
		.Replace("mm", "{1:##}")
		.Replace("ss", "{2:##}")
		.Replace("h", "{0}")
		.Replace("m", "{1}")
		.Replace("s", "{2}");
	s = Format(s, Hours(), Minutes(), Seconds());
	if (s.Contains("nnn"))
		s = Format(s.Replace("nnn", "{0}"), Milliseconds());
	if (s.Contains("nn"))
		s = Format(s.Replace("nn", "{0}"), Milliseconds() / 10);
	if (s.Contains("n"))
		s = Format(s.Replace("n", "{0}"), Milliseconds() / 100);
	return s;
}

// TimeSpan::ISerializable

Boolean TimeSpan::FromFormat(const String& value, const String& format)
{
	return TimeParse(value, data);
}

}
}
