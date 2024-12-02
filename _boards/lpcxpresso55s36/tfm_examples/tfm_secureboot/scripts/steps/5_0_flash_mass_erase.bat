@echo off
@rem - The script for Flash mass erase -

@rem -- Set variables --
call ..\config.cmd
if errorlevel 1 goto ERROR

pushd "%SPT_WORKSPACE%"

@echo ### Flash mass erase, including the Protected Flash Region. ###
call "%nxpdebugmbox%" erase
if errorlevel 1 goto ERROR

@echo SUCCESS
    popd
	if not defined NO_PAUSE (pause)
	exit /b 0
:ERROR
    popd
    if not defined NO_PAUSE (pause)
    exit /b 2
