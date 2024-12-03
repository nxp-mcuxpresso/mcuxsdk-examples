@echo off
setlocal
SET CURRENT_DIR=%~dp0
SET CFPA=1
echo %CURRENT_DIR%
rem Check if the input file is present.
if not exist %CURRENT_DIR%output\signed_tfm_secure_boot.bin (
    echo [ERROR] %CURRENT_DIR%output\signed_tfm_secure_boot.bin does not exist!
    exit /b 1
)
rem Check if the cfpa binary is present.
set CONTINUE=Y
if not exist %CURRENT_DIR%output\signed_tfm_cfpa.bin (
    echo [WARNING] %CURRENT_DIR%output\signed_tfm_cfpa.bin does not exist!
	set CFPA=0
	set /P CONTINUE=Are you sure you want to continue[Y/N]?
)
if /I "%CONTINUE%" neq "Y" goto :end_script

cd ..\..\..\..\..\middleware\tfm\tf-m\platform\ext\target\nxp\common\secure_boot\script
rem The inputs for batch files are:
rem                   1. choose creation of sb file for secure boot or for cfpa application 
rem                   2. device
rem                   3. location of signed binary
rem                   4. location of cfpa file (only for cfpa application)
rem                   5. location of output sb2 file
@echo on
call make_sb.bat -secure_boot -d lpc55S1x -b "%CURRENT_DIR%output\signed_tfm_secure_boot.bin" -o "%CURRENT_DIR%output\tfm_secure_boot.sb2"
if %CFPA%==1 (
    call make_sb.bat -cfpa -d lpc55S1x -b "%CURRENT_DIR%output\signed_tfm_cfpa.bin" -c "%CURRENT_DIR%output\cfpa.bin" -o "%CURRENT_DIR%output\tfm_cfpa.sb2"
) 
cd %CURRENT_DIR%
:end_script
endlocal