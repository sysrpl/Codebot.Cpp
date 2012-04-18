unit CoreTools;

{$ifdef fpc}
{$mode delphi}
{$endif}

interface

uses
	SysUtils, ShareTools;

{ Command line information }

function CommandLineCount: Integer; cdecl;
function CommandLineArgument(Index: Integer): PAnsiChar; cdecl;

{ Basic type parsing routines }

function IntegerParse(S: PAnsiChar; var Value: Integer): Boolean; cdecl;
function SingleParse(S: PAnsiChar; var Value: Single): Boolean; cdecl;
function DoubleParse(S: PAnsiChar; var Value: Double): Boolean; cdecl;

{ Atomic functions }

function AtomicIncrement(var Value: Integer): Integer; cdecl;
function AtomicDecrement(var Value: Integer): Integer; cdecl;

{ File routines }

function FileExists(FileName: PAnsiChar): Boolean; cdecl;
function FileDelete(FileName: PAnsiChar): Boolean; cdecl;
function FileRename(OldName, NewName: PAnsiChar): Boolean; cdecl;

{ Directory routines }

function DirectoryExists(Dir: PAnsiChar): Boolean; cdecl;
function DirectoryCurrent: PAnsiChar; cdecl;
function DirectoryChange(Dir: PAnsiChar): Boolean; cdecl;
function DirectoryCreate(Dir: PAnsiChar): Boolean; cdecl;
function DirectoryDelete(Dir: PAnsiChar): Boolean; cdecl;
function DirectoryRename(OldName, NewName: PAnsiChar): Boolean; cdecl;

{ Other routines }

procedure GuidCreate(out Guid: TGUID); cdecl;

implementation

{$ifndef fpc}
uses
	Windows;
{$endif}

{ Command line information }

function CommandLineCount: Integer;
begin
	Result := ParamCount + 1;
end;

function CommandLineArgument(Index: Integer): PAnsiChar;
begin
	Return(Result, ParamStr(Index));
end;

{ Atomic functions }

function AtomicIncrement(var Value: Integer): Integer;
begin
	Result := InterlockedIncrement(Value);
end;

function AtomicDecrement(var Value: Integer): Integer;
begin
	Result := InterlockedDecrement(Value);
end;

{ File routines }

function FileExists(FileName: PAnsiChar): Boolean;
begin
	Result := SysUtils.FileExists(FileName);
end;

function FileDelete(FileName: PAnsiChar): Boolean;
begin
	Result := DeleteFile(FileName);
end;

function FileRename(OldName, NewName: PAnsiChar): Boolean;
begin
	Result := RenameFile(OldName, NewName);
end;

{ Directory routines }

function DirectoryExists(Dir: PAnsiChar): Boolean;
begin
	Result := SysUtils.DirectoryExists(Dir);
end;

function DirectoryCreate(Dir: PAnsiChar): Boolean;
begin
	Result := SysUtils.CreateDir(Dir);
end;

function DirectoryDelete(Dir: PAnsiChar): Boolean;
begin
	Result := SysUtils.RemoveDir(Dir);
end;

function DirectoryRename(OldName, NewName: PAnsiChar): Boolean;
begin
	Result := SysUtils.RenameFile(OldName, NewName);
end;

function DirectoryCurrent: PAnsiChar;
begin
	Return(Result, GetCurrentDir);
end;

function DirectoryChange(Dir: PAnsiChar): Boolean;
begin
	Result := SetCurrentDir(Dir);
end;

{ Basic type parsing routines }

function IntegerParse(S: PAnsiChar; var Value: Integer): Boolean;
begin
	Result := TryStrToInt(S, Value);
end;

function SingleParse(S: PAnsiChar; var Value: Single): Boolean;
begin
	Result := TryStrToFloat(S, Value);
end;

function DoubleParse(S: PAnsiChar; var Value: Double): Boolean;
begin
	Result := TryStrToFloat(S, Value);
end;

{ Other routines }

procedure GuidCreate(out Guid: TGUID);
begin
	CreateGuid(Guid);
end;

end.
