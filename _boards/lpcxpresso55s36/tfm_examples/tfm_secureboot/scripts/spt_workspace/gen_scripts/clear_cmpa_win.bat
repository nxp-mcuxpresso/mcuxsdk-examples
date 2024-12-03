@echo off

@rem The script for clearing CMPA page for LPC55S3x

@rem Environment variables - absolute paths simplifying re-use of the script on another machine
@rem  - Absolute path to SPT workspace (it is recommended to copy/move all user files in this workspace)
SETLOCAL ENABLEDELAYEDEXPANSION
if "%SPT_WORKSPACE%"=="" (
  SET "SPT_WORKSPACE=%~dp0"
  SET "SPT_WORKSPACE=!SPT_WORKSPACE:~0,-1!"
  FOR %%i in ("!SPT_WORKSPACE!") DO (
    SET "SPT_WORKSPACE=%%~dpi"
  )
  SET "SPT_WORKSPACE=!SPT_WORKSPACE:~0,-1!"
)
ENDLOCAL & SET "SPT_WORKSPACE=%SPT_WORKSPACE%"
@rem  - Absolute path to SPT installation directory
if "%SPT_INSTALL_BIN%"=="" (
  SET "SPT_INSTALL_BIN=C:\nxp\MCUX_Provi_v5\bin"
)


@rem Default connection parameters
set blhost_connect=-u 0x1FC9,0x0025

@rem Used command line utilities
set "blhost=%SPT_INSTALL_BIN%\tools_scripts\blhost_spsdk_win_wrapper.bat"

@echo ### Check connection ###
call "%blhost%" %blhost_connect% -j -- get-property 1 0
if errorlevel 2 exit 2

@echo ### Clear CMPA ###
call "%blhost%" %blhost_connect% -j -- write-memory 0x0003E200  "%SPT_INSTALL_BIN%\data\script_templates\LPC55S3x\cmpa_empty.bin"
if errorlevel 1 exit 2

@echo ### Reset ###
call "%blhost%" %blhost_connect% -j -- reset
if errorlevel 2 exit 2