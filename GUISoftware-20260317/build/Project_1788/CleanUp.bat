@ECHO OFF

ECHO CleanUp.BAT:         Deleting output

REM ****************************************
REM   Move file
REM ****************************************

move Debug\Exe\*.hex ..\..\

REM ****************************************
REM   Delete working folders
REM ****************************************

IF "%OS%" == "Windows_NT" GOTO WinNT
FOR %%i IN (Source, Debug, Release) DO DELTREE %%i
GOTO CONT2
:WinNT
FOR %%i IN (Source, Debug, Release) DO IF EXIST %%i RD %%i /S/Q
:CONT2

REM ****************************************
REM   Delete files
REM ****************************************

FOR %%i IN (O, PBI) DO IF EXIST *.%%i DEL *.%%i

