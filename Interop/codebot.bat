@ECHO OFF
dcc32.exe %1.dpr
if %ERRORLEVEL% GEQ 1 goto ERROR
lib /MACHINE:X86 /DEF:%1.def /OUT:%1.lib
if %ERRORLEVEL% GEQ 1 goto ERROR
copy %1.dll %2\Lib
if %ERRORLEVEL% GEQ 1 goto ERROR
copy %1.lib %2\Lib
if %ERRORLEVEL% GEQ 1 goto ERROR
copy %1.exp %2\Lib
if %ERRORLEVEL% GEQ 1 goto ERROR
copy %1.def %2\Lib
if %ERRORLEVEL% GEQ 1 goto ERROR
copy %1.dll %2\Debug
if %ERRORLEVEL% GEQ 1 goto ERROR
copy %1.dll %2\Release
if %ERRORLEVEL% GEQ 1 goto ERROR
:SUCCESS
goto END
:ERROR
echo Failure
:END
