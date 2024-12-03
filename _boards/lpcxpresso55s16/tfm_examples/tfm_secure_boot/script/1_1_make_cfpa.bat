@echo off
setlocal
SET CURRENT_DIR=%~dp0
echo %CURRENT_DIR%
if exist "%CURRENT_DIR%pfr" rmdir "%CURRENT_DIR%pfr"
mkdir "%CURRENT_DIR%pfr"
cd ..\..\..\..\..\middleware\tfm\tf-m\platform\ext\target\nxp\common\secure_boot\script
rem The inputs for batch files are:
rem                   1. device
rem                   2. path, where the output binary file (cfpa table) will be stored
rem                   3. parameter "allPFR" or "cfpa"
@echo on
call make_pfr.bat -d lpc55s1x -p %CURRENT_DIR%pfr -c cfpa
cd %CURRENT_DIR%  
