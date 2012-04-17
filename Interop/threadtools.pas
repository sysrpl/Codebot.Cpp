unit ThreadTools;

{$ifdef fpc}
{$mode delphi}
{$endif}

interface

uses
	SysUtils, Classes;

{ Threading }

type
	TThreadProc = procedure(Param: Pointer); cdecl;

function ThreadStart(Proc: TThreadProc; Param: Pointer): THandle; cdecl;
function ThreadWait(var Thread: THandle): Boolean; cdecl;
procedure ThreadSleep(Time: Cardinal); cdecl;

implementation

type
	TToolThread = class(TThread)
	protected
		FProc: TThreadProc;
		FParam: Pointer;
		procedure Execute; override;
	public
		constructor Create(Proc: TThreadProc; Param: Pointer);
	end;

constructor TToolThread.Create(Proc: TThreadProc; Param: Pointer);
begin
	FProc := Proc;
	FParam := Param;
	inherited Create(False);
end;

procedure TToolThread.Execute; 
begin
	FProc(FParam);
end;

function ThreadStart(Proc: TThreadProc; Param: Pointer): THandle;
begin
	Result := THandle(TToolThread.Create(Proc, Param));
end;

function ThreadWait(var Thread: THandle): Boolean;
var
	H: THandle;
begin
	Result := Thread <> 0;
	if Result then
	begin
		H := Thread;
		Thread := 0;
		TThread(H).WaitFor;
		TThread(H).Free;
	end;
end;

procedure ThreadSleep(Time: Cardinal);
begin
	Sleep(Time);
end;

end.
