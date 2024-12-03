@echo off
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
if not exist "%SPT_WORKSPACE%\" (
  @echo FAILURE: Directory not found "%SPT_WORKSPACE%\"
  exit 2
)
@rem  - Absolute path to SPT installation directory
if "%SPT_INSTALL_BIN%"=="" (
  SET "SPT_INSTALL_BIN=C:\nxp\MCUX_Provi_v6\bin"
)
if not exist "%SPT_INSTALL_BIN%\" (
  @echo FAILURE: Directory not found "%SPT_INSTALL_BIN%\"
  exit 2
)

@rem Used command line utilities
SET "nxpdebugmbox=%SPT_INSTALL_BIN%\tools\spsdk\nxpdebugmbox.exe"
pushd "%SPT_WORKSPACE%"
set /A return_success=0
@echo "%nxpdebugmbox%" -p 2.1 -i pyocd -s UNCEWCZ3KXW2X auth -b 0 -c "%SPT_WORKSPACE%\debug_auth\debug_auth_cert.dc" -k "%SPT_WORKSPACE%\keys\debug_authentication_key_ECC_384.pem"
call "%nxpdebugmbox%" -p 2.1 -i pyocd -s UNCEWCZ3KXW2X auth -b 0  ^
    -c "%SPT_WORKSPACE%\debug_auth\debug_auth_cert.dc"  ^
    -k "%SPT_WORKSPACE%\keys\debug_authentication_key_ECC_384.pem"
if not errorlevel 1 (
    if errorlevel 0 (
        set /A return_success=1
    )
)
popd
if %return_success% == 1 (
    echo nxpdebugmbox succeeded
    exit /B 0
)
echo nxpdebugmbox failed
exit /B 2