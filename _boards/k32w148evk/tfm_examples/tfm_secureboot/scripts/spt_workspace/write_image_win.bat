@echo off

@rem The script for writing the image into K32W1xx

@rem Environment variables - absolute paths simplifying re-use of the script on another machine
@rem  - Absolute path to SPT workspace (it is recommended to copy/move all user files in this workspace)
SETLOCAL ENABLEDELAYEDEXPANSION
if "%SPT_WORKSPACE%"=="" (
  SET "SPT_WORKSPACE=%~dp0"
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

@rem Use parameter: "blhost_connect <connection_param>" to use custom connection parameters for blhost
@rem     <connection_param> should be in format "-p COMx[,baud]" or "-u VID,PID" and must be enclosed in quotes
@rem Use parameter: "manufacturing_task_no index" to set number/index of the manufacturing task (to avoid same filename created from two tasks)
@rem Use parameter: "erase_all" to perform an erase of the entire flash memory instead erasing only regions that will be written
@rem Use parameter: "write_romcfg" to write IFR ROM configuration (!!irreversible operation!!)
@rem Use parameter: "burn_LC_state" to burn LC state fuse
@rem Use parameter: "burn_rotkth" to burn Root of Trust Key Table Hash (RoTKTH) fuse
@rem Use parameter: "burn_SB3_KDK" to burn SB3 Key Derivation Key fuse

@rem Default connection parameters
set blhost_connect=-p COM11,57600
set "manufacturing_task_no="

@echo ### Parse input arguments ###
:test_param_loop
if [%1]==[] (
  goto test_param_end
)
set param=%1
set known_param=0
if "%param%"=="blhost_connect" (
    set blhost_connect=%~2
    SHIFT
    set known_param=1
)
if "%param%"=="manufacturing_task_no" (
    set manufacturing_task_no=%~2
    SHIFT
    set known_param=1
)
if "%param%"=="erase_all" (
    set erase_all=1
    set known_param=1
)
if "%param%"=="write_romcfg" (
    set write_romcfg=1
    set known_param=1
)
if "%param%"=="burn_LC_state" (
    set burn_LC_state=1
    set known_param=1
)
if "%param%"=="burn_rotkth" (
    set burn_rotkth=1
    set known_param=1
)
if "%param%"=="burn_SB3_KDK" (
    set burn_SB3_KDK=1
    set known_param=1
)
if %known_param%==0 (
    echo ERROR: unsupported argument "%param%"
    exit /B 2
)
@rem Check for further batch arguments
SHIFT
goto test_param_loop
:test_param_end

@rem Used command line utilities
SET "blhost=%SPT_INSTALL_BIN%\tools_scripts\blhost_spsdk_win_wrapper.bat"

@rem Ping the device to establish communication; Result is ignored as the communication sometime fails for the first time
call "%blhost%" %blhost_connect% -j -- get-property 1 0 > nul 2> nul

@echo ### Check connection ###
call "%blhost%" %blhost_connect% -j -- get-property 1 0
if errorlevel 2 exit 2

@rem === Handle processor in DEPLOYMENT mode ===
@echo ### Check if processor is in DEVELOPMENT mode ###
set "blhost_wrapper_expected_output=3275539260 (0xc33cc33c)"
call "%blhost%" %blhost_connect% -- get-property 17
set "blhost_wrapper_expected_output="
if errorlevel 2 exit 2
if errorlevel 1 (
    @echo ### Processor is in DEPLOYMENT mode; however the application still can be updated using SB file ###

    @echo ### Increase voltage for fuse burning ###
    call "%blhost%" %blhost_connect% -j -- set-property 22 1
    if errorlevel 2 goto reset_voltage_exit2

    @echo ### Load/Update bootable image using SB3 capsule ###
    call "%blhost%" %blhost_connect% -- receive-sb-file "%SPT_WORKSPACE%\bootable_images\tfm_secureboot.sb"
    if errorlevel 2 (
        echo Try to call receive-sb-file with parameter `--check-errors` to detect real problem description
        goto reset_voltage_exit2
    )

    @echo ### Set voltage to normal value ###
    call "%blhost%" %blhost_connect% -j -- set-property 22 0
    if errorlevel 2 exit 2
    exit 0
)

@echo ### Erase flash ###
if [%erase_all%] == [1] (
    call "%blhost%" %blhost_connect% -j -- flash-erase-all 0
) else (
    call "%blhost%" %blhost_connect% -j -- flash-erase-region 0x00000000 1442816 0
)
if errorlevel 1 exit 2

@rem === IFR ROM configuration ===
if [%write_romcfg%] == [1] (
    @echo ### Write IFR ROM configuration ###
    call "%blhost%" %blhost_connect% -j -- write-memory 0x02000000 "%SPT_WORKSPACE%\configs\romcfg.bin"
    if errorlevel 2 exit 2
)

set "sb3_fuse_changed=0"
if [%burn_rotkth%] == [1] (
    @echo ### Burn RoTKTH fuses ###

    @echo ### Increase voltage for fuse burning ###
    call "%blhost%" %blhost_connect% -j -- set-property 22 1
    if errorlevel 2 goto reset_voltage_exit2

    @rem Burn fuse: CUST_PROD_OEMFW_AUTH_PUK[0:255]
    call "%blhost%" %blhost_connect% -j -- fuse-program 0x1f {{650D8097079FF27A3E8A2DA14781B922FD8295B6C00BFA067F00E87F1A16B8B3}}
    if errorlevel 2 goto reset_voltage_exit2

    @echo ### Set voltage to normal value ###
    call "%blhost%" %blhost_connect% -j -- set-property 22 0
    if errorlevel 2 exit 2
    set "sb3_fuse_changed=1"
)
if [%burn_SB3_KDK%] == [1] (
    @echo ### Burn SB3_KDK fuses ###

    @echo ### Increase voltage for fuse burning ###
    call "%blhost%" %blhost_connect% -j -- set-property 22 1
    if errorlevel 2 goto reset_voltage_exit2

    @rem Burn fuse: CUST_PROD_OEMFW_ENC_SK[0:255]
    call "%blhost%" %blhost_connect% -j -- fuse-program 0x20 {{7AA7EF9813B3561257B8837DAB26225301DF3511217F2733C71DADCD447722D1}}
    if errorlevel 2 goto reset_voltage_exit2

    @echo ### Set voltage to normal value ###
    call "%blhost%" %blhost_connect% -j -- set-property 22 0
    if errorlevel 2 exit 2
    set "sb3_fuse_changed=1"
)
if [%burn_LC_state%] == [1] (
    @echo ### Burn LC_STATE fuses ###

    @echo ### Increase voltage for fuse burning ###
    call "%blhost%" %blhost_connect% -j -- set-property 22 1
    if errorlevel 2 goto reset_voltage_exit2

    @rem Burn fuse: LIFECYCLE
    call "%blhost%" %blhost_connect% -j -- fuse-program 0xa {{07}}
    if errorlevel 2 goto reset_voltage_exit2

    @echo ### Set voltage to normal value ###
    call "%blhost%" %blhost_connect% -j -- set-property 22 0
    if errorlevel 2 exit 2
    set "sb3_fuse_changed=1"
)

if [%sb3_fuse_changed%] == [1] (
    @echo ### Reset ###
    call "%blhost%" %blhost_connect% -j -- reset
    @echo ### Delay until processor is restarted; the time can be customized in preferences ###
    setlocal EnableDelayedExpansion
    SET /A "timeout=3 + 1"
    ping -n !timeout! 127.0.0.1 >NUL
    endlocal
    @rem Ping the device to establish communication; Result is ignored as the communication sometime fails for the first time
    call "%blhost%" %blhost_connect% -j -- get-property 1 0 > nul 2> nul

    @echo ### Check connection ###
    call "%blhost%" %blhost_connect% -j -- get-property 1 0
    if errorlevel 2 exit 2
)

@echo ### Increase voltage for fuse burning ###
call "%blhost%" %blhost_connect% -j -- set-property 22 1
if errorlevel 2 goto reset_voltage_exit2

@echo ### Load bootable image using SB3 capsule ###
call "%blhost%" %blhost_connect% -- receive-sb-file "%SPT_WORKSPACE%\bootable_images\tfm_secureboot.sb"
if errorlevel 2 (
    echo Try to call receive-sb-file with parameter `--check-errors` to detect real problem description
    goto reset_voltage_exit2
)

@echo ### Set voltage to normal value ###
call "%blhost%" %blhost_connect% -j -- set-property 22 0
if errorlevel 2 exit 2

exit /B 0

:reset_voltage_exit2
    @echo ### Set voltage to normal value ###
    call "%blhost%" %blhost_connect% -j -- set-property 22 0
    exit 2