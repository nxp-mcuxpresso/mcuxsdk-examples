@echo off
@rem Script to build KW45xx Master Boot Image using nxpimage tool

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

@rem Used command line utilities
SET "nxpimage=%SPT_INSTALL_BIN%\tools_scripts\nxpimage_spsdk_win_wrapper.bat"
SET "ifr=%SPT_INSTALL_BIN%\tools\spsdk\ifr.exe"
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
@echo %align% "%SPT_WORKSPACE%\bootable_images\tfm_secureboot.bin" 721408
call "%align%" "%SPT_WORKSPACE%\bootable_images\tfm_secureboot.bin" 721408
if errorlevel 1 exit 2

@echo ### Create IFR ROM configuration page - binary ###
pushd "%SPT_WORKSPACE%"
@echo %ifr% generate-binary -d kw45xx -c "%SPT_WORKSPACE%\configs\romcfg.json" -o "%SPT_WORKSPACE%\configs\romcfg.bin"
"%ifr%" generate-binary -d kw45xx -c "%SPT_WORKSPACE%\configs\romcfg.json" ^
		-o "%SPT_WORKSPACE%\configs\romcfg.bin"
if errorlevel 1 (
    popd
    exit 2
)
popd

@echo ### Create SB file ###
call "%nxpimage%" sb31 export "%SPT_WORKSPACE%\configs\sb3_config.json"
if errorlevel 2 exit 2