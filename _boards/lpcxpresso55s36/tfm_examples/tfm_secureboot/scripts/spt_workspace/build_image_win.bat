@echo off
@rem Script to build LPC55S3x Master Boot Image using nxpimage tool

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

@rem Use parameter: "no_dev_hsm_provi" to avoid creation and modification of dev_hsm_provisioning.sb file

@echo ### Parse input arguments ###
:test_param_loop
if [%1]==[] (
  goto test_param_end
)
set param=%1
set known_param=0
if "%param%"=="no_dev_hsm_provi" (
    set no_dev_hsm_provi=1
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
SET "nxpimage=%SPT_INSTALL_BIN%\tools_scripts\nxpimage_spsdk_win_wrapper.bat"
SET "pfr=%SPT_INSTALL_BIN%\tools\spsdk\pfr.exe"
SET "nxpdevhsm=%SPT_INSTALL_BIN%\tools\spsdk\nxpdevhsm.exe"
SET "blhost=%SPT_INSTALL_BIN%\tools_scripts\blhost_spsdk_win_wrapper.bat"
SET "align=%SPT_INSTALL_BIN%\tools\utils\win\align.exe"

if not exist "%SPT_WORKSPACE%\bootable_images\." (
    mkdir "%SPT_WORKSPACE%\bootable_images"
    if errorlevel 1 exit 2
)

@echo ### Create bootable image ###
call "%nxpimage%" mbi export "%SPT_WORKSPACE%\configs\mbi_config.json"
if errorlevel 2 exit 2

@echo ### Align bootable image size to PRINCE sub-region boundary ###
@echo %align% "%SPT_WORKSPACE%\bootable_images\tfm_secureboot.bin" 229376
call "%align%" "%SPT_WORKSPACE%\bootable_images\tfm_secureboot.bin" 229376
if errorlevel 1 exit 2

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

@echo ### Create CMPA page - binary ###
pushd "%SPT_WORKSPACE%"
@echo %pfr% generate-binary -c %SPT_WORKSPACE%\configs\cmpa.json -e "%SPT_WORKSPACE%\configs\mbi_config.json" -o %SPT_WORKSPACE%\configs\cmpa.bin 
"%pfr%" generate-binary -c "%SPT_WORKSPACE%\configs\cmpa.json" ^
        -e "%SPT_WORKSPACE%\configs\mbi_config.json" ^
        -o "%SPT_WORKSPACE%\configs\cmpa.bin"
if errorlevel 1 (
    popd
    exit 2
)
popd

@echo ### Create SB file ###
call "%nxpimage%" sb31 export "%SPT_WORKSPACE%\configs\sb3_config.json" 
if errorlevel 2 exit 2

if [%no_dev_hsm_provi%] == [1] (
    exit 0
)
@echo ### Create key-provisioning SB file installing CUST-MK-SK key into processor ###
if not exist "%SPT_WORKSPACE%\configs\nxpdevhsm\." (
    mkdir "%SPT_WORKSPACE%\configs\nxpdevhsm"
    if errorlevel 1 exit 2
)
@echo "%nxpdevhsm%" generate -u 0x1FC9,0x0025 -k "%SPT_WORKSPACE%\configs\cust_mk_sk.bin" -o "%SPT_WORKSPACE%\configs\sb_seed.bin" -w "%SPT_WORKSPACE%\configs\nxpdevhsm" -f lpc55s3x "%SPT_WORKSPACE%\bootable_images\dev_hsm_provisioning.sb"
"%nxpdevhsm%" generate -u 0x1FC9,0x0025 ^
	-k "%SPT_WORKSPACE%\configs\cust_mk_sk.bin" ^
	-o "%SPT_WORKSPACE%\configs\sb_seed.bin" ^
	-w "%SPT_WORKSPACE%\configs\nxpdevhsm" ^
	-f lpc55s3x ^
	"%SPT_WORKSPACE%\bootable_images\dev_hsm_provisioning.sb"
if errorlevel 1 exit 2
