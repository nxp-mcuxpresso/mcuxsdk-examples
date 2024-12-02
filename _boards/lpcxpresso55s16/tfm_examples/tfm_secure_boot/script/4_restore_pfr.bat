echo off

setlocal
set CMD=%~1
if "%CMD%" == "" (
  echo [ERR] Input parameter is missed!
  goto usage
)

SET CURRENT_DIR=%~dp0
echo %CURRENT_DIR%

cd ..\..\..\..\..\middleware\tfm\tf-m\platform\ext\target\nxp\common\secure_boot\script
rem The inputs for batch files are:
rem                   1. COM PORT
rem                   2. Device
rem                   3. address of cfpa table
rem                   4. address of cmpa table
@echo on
restore_pfr.bat -p %1 -d lpc55s1x -a1 0x3de00 -a2 0x3e400
cd %CURRENT_DIR%

:usage
echo Usage: 4_restore_pfr.bat COMx
exit /b 1  
