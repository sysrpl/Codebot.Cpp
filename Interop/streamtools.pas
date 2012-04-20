unit StreamTools;

{$ifdef fpc}
{$mode delphi}
{$endif}

interface

uses
	Classes;

type
	INativeStream = interface
		['{5498E774-B3D1-4C80-95F8-7573EF266E29}']
		function Read(Data: Pointer; Count: Cardinal): Cardinal; stdcall;
		function Write(Data: Pointer; Count: Cardinal): Cardinal; stdcall;
		function Seek(Origin: Cardinal; Cursor: Integer): Cardinal; stdcall;
	end;
	
	TNativeStream = class(TStream)
	private
		FStream: INativeStream;
	public
		constructor Create(Stream: INativeStream);
    function Read(var Buffer; Count: Longint): Longint; override;
    function Write(const Buffer; Count: Longint): Longint; override;
    function Seek(Offset: Longint; Origin: Word): Longint; override;
	end;

implementation

constructor TNativeStream.Create(Stream: INativeStream);
begin
	inherited Create;
	FStream := Stream;
end;

function TNativeStream.Read(var Buffer; Count: Longint): Longint; 
begin
	if Count < 0 then
		Count := 0;
	Result := FStream.Read(@Buffer, Count);
end;

function TNativeStream.Write(const Buffer; Count: Longint): Longint; 
begin
	if Count < 0 then
		Count := 0;
	Result := FStream.Write(@Buffer, Count);
end;

function TNativeStream.Seek(Offset: Longint; Origin: Word): Longint; 
begin
	if Origin > 2 then
		Origin := 2;
	Result := FStream.Seek(Origin, Offset);
end;

end.
