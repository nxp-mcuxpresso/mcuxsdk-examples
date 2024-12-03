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

@echo ### Increase voltage for fuse burning ###
"%blhost%" %connect% -- set-property 22 1
if errorlevel 1 goto ERROR

@echo ### Burn fuse: LC_STATE[7:0] Switch to OEM Secure closed (0xf) state ###
"%blhost%" %connect% -- fuse-program 0xa {{F}}
if errorlevel 1 set issue=1

@echo ### Set voltage to normal value ###
"%blhost%" %connect% -- set-property 22 0
if errorlevel 1 goto ERROR

if %issue%==1 goto ERROR

@echo SUCCESS
:EXIT
    popd
	if not defined NO_PAUSE (pause)
	exit /b 0
:ERROR
@echo FAIL
    popd
    if not defined NO_PAUSE (pause)
    exit /b 2
