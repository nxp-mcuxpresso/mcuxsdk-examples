@echo off
setlocal
SET CURRENT_DIR=%~dp0
echo %CURRENT_DIR%
if not exist "%CURRENT_DIR%output" mkdir "%CURRENT_DIR%output"
cd ..\..\..\..\..\middleware\tfm\tf-m\platform\ext\target\nxp\common\secure_boot\script
@echo on
call make_bin.bat -secure_boot -ns %CURRENT_DIR%..\tfm_secure_boot_ns\mdk\release\tfm_secure_boot_ns.hex -s %CURRENT_DIR%..\tfm_secure_boot_s\mdk\release\tfm_secure_boot_s.hex -o %CURRENT_DIR%output\tfm_secure_boot.bin -a 0x38000
if exist "%CURRENT_DIR%..\tfm_cfpa\mdk\release\tfm_cfpa.hex" (
    call make_bin.bat -cfpa -app %CURRENT_DIR%..\tfm_cfpa\mdk\release\tfm_cfpa.hex -o %CURRENT_DIR%output\tfm_cfpa.bin -a 0x38000
)
cd %CURRENT_DIR%