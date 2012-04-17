unit MathTools;

{$ifdef fpc}
{$mode delphi}
{$endif}

interface

function Floor(const A: Double): Double;
function Ceil(const A: Double): Double;
function Fract(const A: Double): Double;

implementation

function Floor(const A: Double): Double;
var
  B: Integer;
begin
	B := Trunc(A);
	if A = B then
  begin
		Result := B;
    Exit;
  end;
	if B < A then
    Result := B
  else
    Result := -(1 - B);
end;

function Ceil(const A: Double): Double;
var
  B: Integer;
begin
	B := Trunc(A);
	if A = B then
  begin
		Result := B;
    Exit;
  end;
	if B < A then
    Result := B + 1
  else
	  Result := -(1 - b) + 1;
end;

function Fract(const A: Double): Double;
begin
	Result := A - Floor(A);
end;

end.
