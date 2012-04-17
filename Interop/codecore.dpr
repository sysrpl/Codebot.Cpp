library codecore;

uses
  MathTools in 'MathTools.pas',
  CoreTools in 'CoreTools.pas',
  DateTools in 'DateTools.pas',
  ShareTools in 'ShareTools.pas';

exports
  { Command line information }
  CommandLineCount,
  CommandLineArgument,
	{ Basic type parsing routines }
	IntegerParse,
	SingleParse,
	DoubleParse,
	{ Module routines }
	ModuleLoad,
	ModuleUnload,
  ModuleFindProc,
	{ Atomic functions }
	AtomicIncrement,
	AtomicDecrement,
	{ File routines }
	FileExists,
	FileSize,
	FileCopy,
	FileDelete,
	FileRename,
	{ Directory routines }
	DirectoryExists,
	DirectoryCurrent,
	DirectoryChange,
	DirectoryCreate,
	DirectoryDelete,
	DirectoryRename,
	{ Path routines }
	PathCombine,
	PathExists,
	{ Time routines }
	TimeParse,
	TimeNow,
	TimeDecode,
	TimeEncode,
	TimeAddHour,
	TimeAddMinute,
	TimeAddSecond,
	TimeAddMillisecond,
	TimeHours,
	TimeMinutes,
	TimeSeconds,
	TimeMilliseconds,
	{ Date routines }
	DateParse,
	DateNow,
	DateDecode,
	DateEncode,
	DateYearDiff,
	DateMonthDiff,
	DateDayDiff,
	DateAddYear,
	DateAddMonth,
	DateAddDay,
	MonthLongName,
	MonthShortName,
	DayLongName,
	DayShortName,
	{ Extraction of Date and Time values }
	YearOfDate,
	MonthOfYear,
	MonthFirstDay,
	MonthLastDay,
	MonthFirstDate,
	MonthLastDate,
	DayOfYear,
	DayOfMonth,
	DayOfWeek,
  { Threading }
  ThreadStart,
  ThreadWait,
  ThreadSleep,
  { Other routines }
  GuidCreate;

begin
end.
