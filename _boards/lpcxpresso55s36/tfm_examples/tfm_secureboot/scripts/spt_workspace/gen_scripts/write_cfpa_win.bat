@echo off

@rem The script for writing the CFPA page into LPC55Sxx
@rem Input file: %SPT_WORKSPACE%\configs\cfpa.json

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
set "pfr=%SPT_INSTALL_BIN%\tools\spsdk\pfr.exe"

@echo ### Create CFPA page - binary ###
pushd "%SPT_WORKSPACE%"
@echo %pfr% generate-binary -c \"%SPT_WORKSPACE%\configs\cfpa.json\" -o \"%SPT_WORKSPACE%\configs\cfpa.bin\"
"%pfr%" generate-binary -c "%SPT_WORKSPACE%\configs\cfpa.json" ^
		-o "%SPT_WORKSPACE%\configs\cfpa.bin"
if errorlevel 1 (
    popd
    exit 2
)
popd

@rem Ping the device to establish communication; Result is ignored as the communication sometime fails for the first time
call "%blhost%" %blhost_connect% -j -- get-property 1 0 > nul

@echo ### Check connection ###
call "%blhost%" %blhost_connect% -j -- get-property 1 0
if errorlevel 2 exit 2

@echo ### Write Customer Field Programmable Area [CFPA] ###
call "%blhost%" %blhost_connect% -j -- write-memory 0x0003DC00 "%SPT_WORKSPACE%\configs\cfpa.bin"
if errorlevel 1 exit 2
