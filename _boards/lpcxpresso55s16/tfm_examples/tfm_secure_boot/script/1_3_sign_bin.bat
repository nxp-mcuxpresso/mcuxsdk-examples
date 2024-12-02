@echo off
setlocal
SET CURRENT_DIR=%~dp0
echo %CURRENT_DIR%
rem Check if the input file is present.
if not exist %CURRENT_DIR%output\tfm_secure_boot.bin (
    echo [ERROR] %CURRENT_DIR%output\tfm_secure_boot.bin does not exist!
    exit /b 1
)
cd ..\..\..\..\..\middleware\tfm\tf-m\platform\ext\target\nxp\common\secure_boot\script
@echo on
rem The inputs for batch files are:
rem                   1. device
rem                   2. path, where the input binary file (application) is stored
call sign_bin.bat -d lpc55s1x -i %CURRENT_DIR%output\tfm_secure_boot.bin
if exist %CURRENT_DIR%output\tfm_cfpa.bin (
    call sign_bin.bat -d lpc55s1x -i %CURRENT_DIR%output\tfm_cfpa.bin
)
cd %CURRENT_DIR%