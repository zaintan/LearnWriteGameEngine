@echo off
call vendor\premake\bin\premake5.exe vs2015
IF %ERRORLEVEL% NEQ 0 (
  PAUSE
)