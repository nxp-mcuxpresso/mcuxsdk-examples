@echo off
@rem -- The script for the secure seal. --

@rem -- Set variables. --
call ..\config.cmd
if errorlevel 1 goto ERROR

pushd "%SPT_WORKSPACE%"

choice /C NY /T 10 /D N /M "WARNING: Do you want to change the device Life Cycle (irreversible operation) "
if %ERRORLEVEL% neq 2 goto EXIT

@rem -- Ping the device to establish communication. --
@echo ### Check connection ###
"%blhost%" %connect% -- get-property 1 0
if errorlevel 1 goto ERROR

@echo ### Burn fuse: LC_STATE[15:0] Switch to OEM-Closed (0xf0f0) state ###
"%blhost%" %connect% -- efuse-program-once 0x2d 0x00000f0f
if errorlevel 1 goto ERROR

@echo SUCCESS
:EXIT
    popd
	if not defined NO_PAUSE (pause)
	exit /b 0
:ERROR
    popd
    if not defined NO_PAUSE (pause)
    exit /b 2
