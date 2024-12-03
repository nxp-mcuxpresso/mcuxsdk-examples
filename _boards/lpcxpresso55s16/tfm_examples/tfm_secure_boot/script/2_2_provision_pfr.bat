echo off

setlocal
set CMD=%~1
if "%CMD%" == "" (
  echo [ERR] Input parameter is missed!
  goto usage
)

SET CURRENT_DIR=%~dp0
echo %CURRENT_DIR%
rem Check if the input file cfpaInit.bin is present.
if not exist %CURRENT_DIR%output\cfpaInit.bin (
    echo [ERROR] %CURRENT_DIR%output\cfpaInit.bin does not exist!
    exit /b 1
)
rem Check if the input file cmpa.bin is present.
if not exist %CURRENT_DIR%output\cmpa.bin (
    echo [ERROR] %CURRENT_DIR%output\cmpa.bin does not exist!
    exit /b 1
)
cd ..\..\..\..\..\middleware\tfm\tf-m\platform\ext\target\nxp\common\secure_boot\script
rem The inputs for batch files are:
rem                   1. COM PORT
rem                   2. Device
rem                   3. path, where the input binary file (cfpa table) is stored
rem                   4. path, where the input binary file (cmpa table) is stored
rem                   5. address of cfpa table
rem                   6. address of cmpa table
@echo on
provision_pfr.bat -p %1 -d lpc55s1x -i1 %CURRENT_DIR%output\cfpaInit.bin -i2 %CURRENT_DIR%output\cmpa.bin -a1 0x3de00 -a2 0x3e400
cd %CURRENT_DIR%

:usage
echo Usage: 2_2_provision_pfr.bat COMx
exit /b 1  