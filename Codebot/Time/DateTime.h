#pragma once
#include "Codebot/Time/TimeSpan.h"
#include "Codebot/Interop/DateInterop.h"

namespace Codebot
{
namespace Time
{

// DateTime type

class DateTime : public ValueType, public ISerializable
{
private:
	Interop::Date data;
public:
	friend class TimeSpan;
	static const DateTime Empty;
	static DateTime Now();
	static DateTime FromInterop(const Interop::Date& d);
	DateTime();
	DateTime(Word year, Word month = 1, Word day = 1);
	DateTime(const DateTime& d);
	DateTime& operator = (const DateTime& d);
	DateTime& operator += (const TimeSpan& t);
	DateTime& operator -= (const TimeSpan& t);
	TimeSpan operator - (const DateTime& d) const;
	DateTime operator + (const TimeSpan& t) const;
	DateTime operator - (const TimeSpan& t) const;
	Boolean operator == (const DateTime& d) const;
	Boolean operator != (const DateTime& d) const;
	Boolean operator > (const DateTime& d) const;
	Boolean operator >= (const DateTime& d) const;
	Boolean operator < (const DateTime& d) const;
	Boolean operator <= (const DateTime& d) const;
	DateTime AddYears(Integer y) const;
	DateTime AddMonths(Integer m) const;
	DateTime AddDays(Integer d) const;
	DateTime AddHours(Integer h) const;
	DateTime AddMinutes(Integer m) const;
	DateTime AddSeconds(Integer s) const;
	DateTime AddMilliseconds(Integer m) const;
	TimeSpan Time();
	Integer YearDifference(const DateTime& d) const;
	Integer Years() const;
	Integer Months() const;
	Integer Days() const;
	Integer Hours() const;
	Integer Minutes() const;
	Integer Seconds() const;
	Integer Milliseconds() const;
	Integer DayOfYear() const;
	Integer DayOfWeek() const;
	Integer DaysInMonth() const;
	// ISerializable
	String ToFormat(const String& format) const;
	Boolean FromFormat(const String& value, const String& format);
};

}
}
