#include "Codebot/Time/DateTime.h"
#include "Codebot/Text/Format.h"

namespace Codebot
{
namespace Time
{

using namespace Interop;
using namespace Text;

const DateTime DateTime::Empty;

DateTime DateTime::Now()
{
	DateTime r;
	r.data = DateNow();
	return r;
}

DateTime DateTime::FromInterop(const Interop::Date& d)
{
	DateTime r;
	r.data = d;
	return r;
}


DateTime::DateTime()
	: data(0)
{
}

DateTime::DateTime(Word year, Word month, Word day)
{
	if  (!DateEncode(data, year, month, day))
	{
		String arg;
		if (year < 0 || year > 10000)
			arg = "year";
		else if (month < 1 || month > 12)
			arg = "month";
		else
			arg = "day";
		ThrowArgumentException(ThisMethod, arg);
	}
}

DateTime::DateTime(const DateTime& d)
{
	data = d.data;
}

DateTime& DateTime::operator = (const DateTime& d)
{
	data = d.data;
	return *this;
}

DateTime& DateTime::operator += (const TimeSpan& t)
{
	data += t.data;
	return *this;
}

DateTime& DateTime::operator -= (const TimeSpan& t)
{
	data -= t.data;
	return *this;
}

TimeSpan DateTime::operator - (const DateTime& d) const
{
	TimeSpan r;
	r.data = data - d.data;
	return r;
}

DateTime DateTime::operator + (const TimeSpan& t) const
{
	DateTime r;
	r.data = data + t.data;
	return r;
}

DateTime DateTime::operator - (const TimeSpan& t) const
{
	DateTime r;
	r.data = data - t.data;
	return r;
}

Boolean DateTime::operator == (const DateTime& d) const
{
	return data == d.data;
}

Boolean DateTime::operator != (const DateTime& d) const
{
	return data != d.data;
}

Boolean DateTime::operator > (const DateTime& d) const
{
	return data > d.data;
}

Boolean DateTime::operator >= (const DateTime& d) const
{
	return data >= d.data;
}

Boolean DateTime::operator < (const DateTime& d) const
{
	return data < d.data;
}

Boolean DateTime::operator <= (const DateTime& d) const
{
	return data <= d.data;
}

DateTime DateTime::AddYears(Integer y) const
{
	DateTime r = *this;
	r.data = DateAddYear(r.data, y);
	return r;
}

DateTime DateTime::AddMonths(Integer m) const
{
	DateTime r = *this;
	r.data = DateAddMonth(r.data, m);
	return r;
}

DateTime DateTime::AddDays(Integer d) const
{
	DateTime r = *this;
	r.data = DateAddDay(r.data, d);
	return r;
}

DateTime DateTime::AddHours(Integer h) const
{
	DateTime r = *this;
	r.data = TimeAddHour(r.data, h);
	return r;
}

DateTime DateTime::AddMinutes(Integer m) const
{
	DateTime r = *this;
	r.data = TimeAddMinute(r.data, m);
	return r;
}

DateTime DateTime::AddSeconds(Integer s) const
{
	DateTime r = *this;
	r.data = TimeAddSecond(r.data, s);
	return r;
}

DateTime DateTime::AddMilliseconds(Integer m) const
{
	DateTime r = *this;
	r.data = TimeAddMillisecond(r.data, m);
	return r;
}

TimeSpan DateTime::Time()
{
	TimeSpan t;	
	t.data = Fract(data);
	return t;
}

Integer DateTime::YearDifference(const DateTime& d) const

{
	Integer y = Years() - d.Years();
	Integer m = Months() - d.Months();
	if (m > 0)
		return y;
	Integer n = Days() - d.Days();
	if ((m == 0) && (n >= 0))
		return y;
	return y - 1;
}

Integer DateTime::Years() const
{
	return YearOfDate(data);
}

Integer DateTime::Months() const
{
	return MonthOfYear(data);
}

Integer DateTime::Days() const
{
	return DayOfMonth(data);
}

Integer DateTime::Hours() const
{
	return TimeHours(data);
}

Integer DateTime::Minutes() const
{
	return TimeMinutes(data);
}

Integer DateTime::Seconds() const
{
	return TimeSeconds(data);
}

Integer DateTime::Milliseconds() const
{
	return TimeMilliseconds(data);
}

Integer DateTime::DayOfYear() const
{
	return Interop::DayOfYear(data);
}

Integer DateTime::DayOfWeek() const
{
	return Interop::DayOfWeek(data);
}

Integer DateTime::DaysInMonth() const
{
	return Interop::MonthLastDay(data);
}

// DateTime::ToFormat types 

class YearString : public ValueType
{
private:
	DateTime data;
public:
	YearString(const DateTime& d);
	String ToFormat(const String& format) const;
};

YearString::YearString(const DateTime& d)
	: data(d)
{
}

String YearString::ToFormat(const String& format) const
{
	Integer i = data.Years();
	Integer y = i / 100;
	if (format == "~~")
		return Format("{0:##}", i - y * 100);
	else
		return AnyToStr(i);
}

class MonthString : public ValueType
{
private:
	DateTime data;
public:
	MonthString(const DateTime& d);
	String ToFormat(const String& format) const;
};

MonthString::MonthString(const DateTime& d)
	: data(d)
{
}

String MonthString::ToFormat(const String& format) const
{
	auto m = data.Months();
	if (format == "~~~~")
		return MonthLongName(m);
	else if (format == "~~~")
		return MonthShortName(m);
	else if (format == "~~")
		return FormatValue(m, "##");
	else
		return AnyToStr(m);
}

class DayString : public ValueType
{
private:
	DateTime data;
public:
	DayString(const DateTime& d);
	String ToFormat(const String& format) const;
};

DayString::DayString(const DateTime& d)
	: data(d)
{
}

String DayString::ToFormat(const String& format) const
{
	auto d = data.DayOfWeek();
	if (format == "~~~~")
		return DayLongName(d);
	else if (format == "~~~")
		return DayShortName(d);
	else if (format == "~~")
		return FormatValue(data.Days(), "##");
	else 	
		return AnyToStr(data.Days());
}

// DateTime::ToFormat 

String DateTime::ToFormat(const String& format) const
{
	String s = format.IsEmpty() ? "M/D/YYYY" : format;
	TimeSpan t = *this;
	String ampm = "am";
	if (s.Contains("am/pm") || s.Contains("AM/PM"))
	{
		s = s.Replace("am/pm", "{4}");
		s = s.Replace("AM/PM", "{5}");
		if (t.Hours() >= 12)
		{
			t = t.AddHours(-12);
			ampm = "pm";
		}
		else if (t.Hours() == 0)
			t = t.AddHours(12);
	}
	String AMPM = ampm.ToUpper();
	YearString y(*this);
	MonthString m(*this);
	DayString d(*this);
	s = s
		.Replace("YYYY", "{0:~~~~}")
		.Replace("YY", "{0:~~}")
		.Replace("MMMM", "{1:~~~~}")
		.Replace("MMM", "{1:~~~}")
		.Replace("MM", "{1:~~}")
		.Replace("M", "{1}")
		.Replace("DDDD", "{2:~~~~}")
		.Replace("DDD", "{2:~~~}")
		.Replace("DD", "{2:~~}")
		.Replace("D", "{2}")
		.Replace("hh", "{3:HH}")
		.Replace("h", "{3:H}")
		.Replace("mm", "{3:MM}")
		.Replace("m", "{3:M}")
		.Replace("ss", "{3:SS}")
		.Replace("s", "{3:S}")
		.Replace("nnn", "{3:NNN}")
		.Replace("nn", "{3:NN}")
		.Replace("n", "{3:N}")
		.Replace("{3:HH}", "{3:hh}")
		.Replace("{3:H}", "{3:h}")
		.Replace("{3:MM}", "{3:mm}")
		.Replace("{3:M}", "{3:m}")
		.Replace("{3:SS}", "{3:ss}")
		.Replace("{3:S}", "{3:s}")
		.Replace("{3:NNN}", "{3:nnn}")
		.Replace("{3:NN}", "{3:nn}")
		.Replace("{3:N}", "{3:n}");
	if (s.Contains("{4}"))
		s = s.Replace("{4}", ampm);
	if (s.Contains("{5}"))
		s = s.Replace("{5}", AMPM);
	return Format(s, y, m, d, t);
}

// DateTime::ISerializable

Boolean DateTime::FromFormat(const String& value, const String& format)
{
	return DateParse(value, data);
}

}
}
