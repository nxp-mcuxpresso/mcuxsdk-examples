echo off

setlocal
set CMD=%~1
if "%CMD%" == "" (
  echo [ERR] Input parameter is missed!
  goto usage
)

SET CURRENT_DIR=%~dp0
echo %CURRENT_DIR%
rem Check if the input file tfm_secure_boot.sb2 is present.
if not exist %CURRENT_DIR%output\tfm_secure_boot.sb2 (
    echo [ERROR] %CURRENT_DIR%output\tfm_secure_boot.sb2 does not exist!
    exit /b 1
)
cd ..\..\..\..\..\middleware\tfm\tf-m\platform\ext\target\nxp\common\secure_boot\script
rem The inputs for batch files are:
rem                   1. COM PORT
rem                   2. Device
rem                   3. path, where the input sb2 file is stored
rem                   4. path, where the cfpa sb2 file is stored (optional)
if not exist %CURRENT_DIR%output\tfm_cfpa.sb2 (
    @echo on
    update_fw.bat -p %1 -d lpc55s1x -i %CURRENT_DIR%output\tfm_secure_boot.sb2
    cd %CURRENT_DIR%
) else (
    @echo on
    update_fw.bat -p %1 -d lpc55s1x -i %CURRENT_DIR%output\tfm_secure_boot.sb2 -i_1 %CURRENT_DIR%output\tfm_cfpa.sb2
    cd %CURRENT_DIR%
)

:usage
echo Usage: 3_update_fw.bat COMx
exit /b 1  