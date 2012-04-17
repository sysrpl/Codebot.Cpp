library codesync;

uses
  SyncTools in 'SyncTools.pas';

exports
  { Thread start and wait from outisde a thread }
  ThreadStart,
  ThreadWait,
  { Routines which can be called from inside a thread }
  ThreadSuspend,
  ThreadResume,
  ThreadSync,
  { Can be called from anywhere}
  Sleep;

begin
end.
