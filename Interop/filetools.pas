unit FileTools; 

{$ifdef fpc}
{$mode delphi}
{$endif}

interface

uses
  SysUtils, ShareTools;

type
  IFileFind = interface
    ['{88CE80B4-CE80-4BCD-B99C-90633A0538B9}']
		procedure Search(Path, Wildcard: PAnsiChar; Attributes: Cardinal); stdcall;
		function Next: Boolean; stdcall;
		procedure Close; stdcall;
		function Name: PAnsiChar; stdcall;
		function Size: LongWord; stdcall;
		function Attributes: LongWord; stdcall;
		function Date: TDateTime; stdcall;		
  end;

procedure FileFindCreate(out Find: IFileFind); cdecl;

implementation

{ TFileFind }

type
	TFileFind = class(TInterfacedObject, IFileFind)
	private
		FSearchRec: TSearchRec;
		FPath: AnsiString;
		FAttributes: Cardinal;
		FOpened: Boolean;
		FFound: Boolean;
	protected
		{ IFileFind }
		procedure Search(Path, Wildcard: PAnsiChar; Attributes: Cardinal); stdcall;
		function Next: Boolean; stdcall;
		procedure Close; stdcall;
		function Name: PAnsiChar; stdcall;
		function Size: LongWord; stdcall;
		function Attributes: LongWord; stdcall;
		function Date: TDateTime; stdcall;		
	public
		destructor Destroy; override;
	end;
	
{ TFileFind }

destructor TFileFind.Destroy;
begin
	Close;
	inherited Destroy;
end;

{ TFndFile.IFileFind }

procedure TFileFind.Search(Path, Wildcard: PAnsiChar; Attributes: Cardinal); 
var
	P, W: AnsiString;
begin
	Close;
	P := PCharToStr(Path);
	W := PCharToStr(Wildcard);
	if P <> '' then
		P := IncludeTrailingPathDelimiter(P);
	FPath := P + W;
	FAttributes := Attributes;
end;

function TFileFind.Next: Boolean; 
begin
	if FOpened then
	begin
		if not FFound then
		begin
			Result := False;
			Exit;
		end;	
		FFound := FindNext(FSearchRec) = 0;
		Result := FFound;
	end
	else
	begin
		FOpened := True;
		FFound := FindFirst(FPath, FAttributes, FSearchRec) = 0;
		Result := FFound;
	end;
end;

procedure TFileFind.Close;
begin
	if FOpened then
		FindClose(FSearchRec);
	FOpened := False;
	FFound := False;
end;

function TFileFind.Name: PAnsiChar; 
begin
	if FFound then
		Return(Result, FSearchRec.Name)
	else
		Return(Result, '');
end;

function TFileFind.Size: LongWord; 
begin
	if FFound then
	begin
		if (FSearchRec.Attr and faDirectory) = faDirectory then
			Result := 0
		else
			Result := FSearchRec.Size;
	end
	else
		Result := 0;
end;

function TFileFind.Attributes: LongWord; 
begin
	if FFound then
		Result := FSearchRec.Attr
	else
		Result := 0;
end;

function TFileFind.Date: TDateTime; 
begin
	if FFound then
		Result := FileDateToDateTime(FSearchRec.Time)
	else
		Result := 0;
end;

procedure FileFindCreate(out Find: IFileFind); 
begin
	Find := TFileFind.Create;
end;

end.
