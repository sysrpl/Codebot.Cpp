library codecore;

{$ifdef fpc}
{$mode delphi}
{$endif}

uses
	{$IFDEF UNIX}
	cthreads,
	{$ENDIF}
	Math,
	ShareTools in 'sharetools.pas',
	CoreTools in 'coretools.pas',
	FileTools  in 'filetools.pas',
	TimeTools in 'timetools.pas',
	ThreadTools in 'threadtools.pas',
	XmlTools in 'xmltools.pas',
	MathTools in 'mathtools.pas';

exports
	{ Command line information }
	CommandLineCount,
	CommandLineArgument,
	{ Basic type parsing routines }
	IntegerParse,
	SingleParse,
	DoubleParse,
	{ Atomic functions }
	AtomicIncrement,
	AtomicDecrement,
	{ File routines }
	FileExists,
	FileDelete,
	FileRename,
	{ Directory routines }
	DirectoryExists,
	DirectoryCurrent,
	DirectoryChange,
	DirectoryCreate,
	DirectoryDelete,
	DirectoryRename,
	{ FileFind interface }
	FileFindCreate,
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
	GuidCreate,
	{ Xml interface }
  XmlDocumentCreate,
	XmlGetCreated,
	XmlGetRefCount;
	
begin
	SetExceptionMask([exInvalidOp, exDenormalized, exZeroDivide, 
		exOverflow, exUnderflow, exPrecision]); 
end.


