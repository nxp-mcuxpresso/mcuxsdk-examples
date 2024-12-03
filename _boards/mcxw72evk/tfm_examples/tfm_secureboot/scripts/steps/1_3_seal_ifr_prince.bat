@echo off
@rem -- The script for writing the IFR config. --

@rem -- Set variables. --
call ..\config.cmd
if errorlevel 1 goto ERROR

pushd "%SPT_WORKSPACE%"

choice /C NY /T 10 /D N /M "WARNING: Do you want to write IFR0 - PRINCE Regions(irreversible operation)"
if %ERRORLEVEL% neq 2 goto EXIT

@rem -- Ping the device to establish communication. --
@echo ### Check connection ###
"%blhost%" %connect% -- get-property 1 0
if errorlevel 1 goto ERROR

@echo ### Write IFR ROM configuration. romcfg.bin is generated at build time ###
call "%blhost%" %connect% -- write-memory 0x02000000 "configs\romcfg.bin"
if errorlevel 2 goto ERROR

@echo ### Reset for IFR configuration to apply###
"%blhost%" %connect% -- reset
@echo ### Delay until processor is restarted;###
setlocal EnableDelayedExpansion
SET /A "timeout=3 + 1"
ping -n !timeout! 127.0.0.1 >NUL
endlocal
@rem Ping the device to establish communication; Result is ignored as the communication sometime fails for the first time
"%blhost%" %connect% -- get-property 1 0 > nul 2> nul

@echo ### Check connection ###
"%blhost%" %connect% -- get-property 1 0
if errorlevel 2 goto ERROR

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
