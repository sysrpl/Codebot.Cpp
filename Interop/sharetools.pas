unit ShareTools;

{$ifdef fpc}
{$mode delphi}
{$endif}

interface

function PCharToStr(S: PAnsiChar): AnsiString;
function StrTrim(const S: AnsiString): AnsiString;
procedure Return(var Result: PAnsiChar; const S: AnsiString);

const
	NewLine = #13#10;
	
implementation

threadvar
  StringBuffer: AnsiString;

function PCharToStr(S: PAnsiChar): AnsiString;
begin
	Result := S;
end;

function StrTrim(const S: AnsiString): AnsiString;
var
  I, L, R: Integer;
begin
  L := 1;
  for I := 1 to Length(S) do
  begin
    L := I;
    if S[I] > ' ' then
      Break;
  end;
  R := Length(S);
  for I := Length(S) downto L do
  begin
    R := I;
    if S[I] > ' ' then
      Break;
  end;
  Result := Copy(S, L, R - L + 1);
end;

procedure Return(var Result: PAnsiChar; const S: AnsiString);
begin
  StringBuffer := S;
  Result := PAnsiChar(StringBuffer);
end;

end.
