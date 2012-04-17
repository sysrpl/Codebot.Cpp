unit ShareTools;

{$ifdef fpc}
{$mode delphi}
{$endif}

interface

function PCharToStr(S: PAnsiChar): AnsiString;
procedure Return(var Result: PAnsiChar; const S: AnsiString);

implementation

threadvar
  StringBuffer: AnsiString;

function PCharToStr(S: PAnsiChar): AnsiString;
begin
	Result := S;
end;

procedure Return(var Result: PAnsiChar; const S: AnsiString);
begin
  StringBuffer := S;
  Result := PAnsiChar(StringBuffer);
end;

end.
