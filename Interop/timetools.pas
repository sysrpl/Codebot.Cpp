unit TimeTools;

{$ifdef fpc}
{$mode delphi}
{$endif}

interface

uses
	SysUtils, DateUtils, ShareTools, MathTools;

{ Time routines }

function TimeParse(S: PAnsiChar; var Value: TDateTime): Boolean; cdecl;

function TimeNow: TDateTime; cdecl;
procedure TimeDecode(var Value: TDateTime; var Hour, Minute, Second, Millisecond: Integer); cdecl;
procedure TimeEncode(var Value: TDateTime; var Hour, Minute, Second, Millisecond: Integer); cdecl;

function TimeAddHour(var Value: TDateTime; var Hour: Integer): TDateTime; cdecl;
function TimeAddMinute(var Value: TDateTime; var Minute: Integer): TDateTime; cdecl;
function TimeAddSecond(var Value: TDateTime; var Second: Integer): TDateTime; cdecl;
function TimeAddMillisecond(var Value: TDateTime; var Millisecond: Integer): TDateTime; cdecl;

function TimeHours(var Value: TDateTime): Integer; cdecl;
function TimeMinutes(var Value: TDateTime): Integer; cdecl;
function TimeSeconds(var Value: TDateTime): Integer; cdecl;
function TimeMilliseconds(var Value: TDateTime): Integer; cdecl;

{ Date routines }

function DateParse(S: PAnsiChar; var Value: TDateTime): Boolean; cdecl;

function DateNow: TDateTime; cdecl;
procedure DateDecode(var Value: TDateTime; var Year, Month, Day: Word); cdecl;
function DateEncode(var Value: TDateTime; var Year, Month, Day: Word): Boolean; cdecl;

function DateYearDiff(var Start, Finish: TDateTime): Integer; cdecl;
function DateMonthDiff(var Start, Finish: TDateTime): Integer; cdecl;
function DateDayDiff(var Start, Finish: TDateTime): Integer; cdecl;

function DateAddYear(var Value: TDateTime; var Year: Integer): TDateTime; cdecl;
function DateAddMonth(var Value: TDateTime; var Month: Integer): TDateTime; cdecl;
function DateAddDay(var Value: TDateTime; var Day: Integer): TDateTime; cdecl;

function MonthLongName(var Month: Word): PAnsiChar; cdecl;
function MonthShortName(var Month: Word): PAnsiChar; cdecl;
function DayLongName(var Day: Word): PAnsiChar; cdecl;
function DayShortName(var Day: Word): PAnsiChar; cdecl;

{ Extraction of Date and Time values }

function YearOfDate(var Value: TDateTime): Word; cdecl;

function MonthOfYear(var Value: TDateTime): Word; cdecl;
function MonthLastDay(var Value: TDateTime): Word; cdecl;
function MonthFirstDate(var Value: TDateTime): TDateTime; cdecl;
function MonthLastDate(var Value: TDateTime): TDateTime; cdecl;

function DayOfYear(var Value: TDateTime): Word; cdecl;
function DayOfMonth(var Value: TDateTime): Word; cdecl;
function DayOfWeek(var Value: TDateTime): Word; cdecl;

implementation

const
	HoursPerDay = 24;
	MinutesPerDay = HoursPerDay * 60;
	SecondsPerDay = MinutesPerDay * 60;
	MillisecondsPerDay = SecondsPerDay * 1000;

{ Time routines }

function TimeParse(S: PAnsiChar; var Value: TDateTime): Boolean;
begin
	Result := TryStrToTime(S, Value);
end;

function TimeNow: TDateTime;
begin
	Result := Time;
end;

procedure TimeDecode(var Value: TDateTime; var Hour, Minute, Second, Millisecond: Integer);
begin
	Hour := TimeHours(Value);
	Minute := TimeMinutes(Value);
	Second := TimeSeconds(Value);
	Millisecond := TimeMilliseconds(Value);
end;

procedure TimeEncode(var Value: TDateTime; var Hour, Minute, Second, Millisecond: Integer);
begin
	Value := Hour / HoursPerDay + Minute / MinutesPerDay +
		Second / SecondsPerDay + Millisecond / MillisecondsPerDay;
end;

function TimeAddHour(var Value: TDateTime; var Hour: Integer): TDateTime;
begin
	Result := Value + Hour / HoursPerDay;
end;

function TimeAddMinute(var Value: TDateTime; var Minute: Integer): TDateTime;
begin
	Result := Value + Minute / MinutesPerDay;
end;

function TimeAddSecond(var Value: TDateTime; var Second: Integer): TDateTime;
begin
	Result := Value + Second / SecondsPerDay;
end;

function TimeAddMillisecond(var Value: TDateTime; var Millisecond: Integer): TDateTime;
begin
	Result := Value + Millisecond / MillisecondsPerDay;
end;

function TimeHours(var Value: TDateTime): Integer;
begin
	Result := Trunc(Value * HoursPerDay);
end;

function TimeMinutes(var Value: TDateTime): Integer;
begin
	Result := Trunc(Value * MinutesPerDay) mod 60;
end;

function TimeSeconds(var Value: TDateTime): Integer;
begin
	Result := Trunc(Value * SecondsPerDay) mod 60;
end;

function TimeMilliseconds(var Value: TDateTime): Integer;
begin
	Result := Trunc(Value * MillisecondsPerDay) mod 1000;
end;

{ DateTime routines }

function DateParse(S: PAnsiChar; var Value: TDateTime): Boolean;
begin
	Result := TryStrToDateTime(S, Value);
end;

function DateNow: TDateTime;
begin
	Result := Now;
end;

procedure DateDecode(var Value: TDateTime; var Year, Month, Day: Word);
begin
	DecodeDate(Value, Year, Month, Day);
end;

function DateEncode(var Value: TDateTime; var Year, Month, Day: Word): Boolean;
begin
	Result := TryEncodeDate(Year, Month, Day, Value);
end;

function DateYearDiff(var Start, Finish: TDateTime): Integer;
begin
	Result := YearsBetween(Start, Finish);
end;

function DateMonthDiff(var Start, Finish: TDateTime): Integer;
begin
	Result := MonthsBetween(Start, Finish);
end;

function DateDayDiff(var Start, Finish: TDateTime): Integer;
begin
	Result := DaysBetween(Start, Finish);
end;

function DateAddYear(var Value: TDateTime; var Year: Integer): TDateTime;
begin
	Result := IncYear(Value, Year);
end;

function DateAddMonth(var Value: TDateTime; var Month: Integer): TDateTime;
begin
	Result := IncMonth(Value, Month);
end;

function DateAddDay(var Value: TDateTime; var Day: Integer): TDateTime;
begin
	Result := IncDay(Value, Day);
end;

function MonthLongName(var Month: Word): PAnsiChar;
var
	S: string;
begin
	if Month in [1..12] then
		S := LongMonthNames[Month]
	else
		S := '';
	Return(Result, S);
end;

function MonthShortName(var Month: Word): PAnsiChar;
var
	S: string;
begin
	if Month in [1..12] then
		S := ShortMonthNames[Month]
	else
		S := '';
	Return(Result, S);
end;

function DayLongName(var Day: Word): PAnsiChar;
var
	S: string;
begin
	if Day in [1..7] then
		S := LongDayNames[Day]
	else
		S := '';
	Return(Result, S);
end;

function DayShortName(var Day: Word): PAnsiChar;
var
	S: string;
begin
	if Day in [1..7] then
		S := ShortDayNames[Day]
	else
		S := '';
	Return(Result, S);
end;

{ Decode routines }

function YearOfDate(var Value: TDateTime): Word;
begin
	Result := YearOf(Value);
end;

function MonthOfYear(var Value: TDateTime): Word;
begin
	Result := MonthOf(Value);
end;

function MonthLastDay(var Value: TDateTime): Word;
begin
	Result := DayOfTheMonth(EndOfTheMonth(Value));
end;

function MonthFirstDate(var Value: TDateTime): TDateTime;
begin
	Result := StartOfTheMonth(Value);
end;

function MonthLastDate(var Value: TDateTime): TDateTime;
begin
	Result := EndOfTheMonth(Value);
end;

function DayOfYear(var Value: TDateTime): Word;
begin
	Result := DayOfTheYear(Value);
end;

function DayOfMonth(var Value: TDateTime): Word;
begin
	Result := DayOf(Value);
end;

function DayOfWeek(var Value: TDateTime): Word;
begin
	Result := DayOfTheWeek(Value) mod 7 + 1;
end;

end.
