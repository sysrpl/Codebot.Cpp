#pragma once
#include <Codebot/Types.h>

namespace Codebot
{
namespace Interop
{

typedef Double Time;
typedef Double Date;

extern "C" 
{	
	// Time functions
	import Time TimeNow();
	import Boolean TimeParse(const PChar s, Time& value);
	import void TimeDecode(const Time& time, Integer& hour, Integer& minute, Integer& second, Integer& millisecond);
	import void TimeEncode(Time& time, const Integer& hour, const Integer& minute, const Integer& second, const Integer& millisecond);
	import Time TimeAddHour(const Time& time, const Integer& hour);
	import Time TimeAddMinute(const Time& time, const Integer& minute);
	import Time TimeAddSecond(const Time& time, const Integer& second);
	import Time TimeAddMillisecond(const Time& time, const Integer& millisecond);
	import Integer TimeHours(const Time& time);
	import Integer TimeMinutes(const Time& time);
	import Integer TimeSeconds(const Time& time);
	import Integer TimeMilliseconds(const Time& time);
	// Date functions
	import Date DateNow();
	import Boolean DateParse(const PChar s, Date& date);
	import void DateDecode(const Date& date, Word& year, Word& month, Word& day);
	import Boolean DateEncode(Date& date, const Word& year, const Word& month, const Word& day);
	import Integer DateYearDiff(Date& start, Date& finish);
	import Integer DateMonthDiff(Date& start, Date& finish);
	import Integer DateDayDiff(Date& start, Date& finish);
	import Date DateAddYear(const Date& date, const Integer& year);
	import Date DateAddMonth(const Date& date, const Integer& month);
	import Date DateAddDay(const Date& date, const Integer& day);
	import PChar MonthLongName(const Word& month);
	import PChar MonthShortName(const Word& month);
	import PChar DayLongName(const Word& day);
	import PChar DayShortName(const Word& day);
	import Word YearOfDate(const Date& value);
	import Word MonthOfYear(const Date& value);
	import Word MonthLastDay(const Date& value);
	import Date MonthFirstDate(const Date& value);
	import Date MonthLastDate(const Date& value);
	import Word DayOfYear(const Date& value);
	import Word DayOfMonth(const Date& value);
	import Word DayOfWeek(const Date& value);
};

}
}
