@echo off
setlocal
SET CURRENT_DIR=%~dp0
echo %CURRENT_DIR%
if not exist "%CURRENT_DIR%output" mkdir "%CURRENT_DIR%output"
cd ..\..\..\..\..\middleware\tfm\tf-m\platform\ext\target\nxp\common\secure_boot\script
rem The inputs for batch files are:
rem                   1. device
rem                   2. path, where the output binary file (pfr table) will be stored
rem                   3. parameter "allPFR" or "cfpa"
@echo on
make_pfr.bat -d lpc55s1x -p %CURRENT_DIR%output -c allPFR
cd %CURRENT_DIR%  