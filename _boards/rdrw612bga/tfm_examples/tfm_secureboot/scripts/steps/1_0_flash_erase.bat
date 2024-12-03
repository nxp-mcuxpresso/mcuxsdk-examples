@echo off
@rem - The script for Flash erase -

@rem -- Set variables --
call ..\config.cmd
if errorlevel 1 goto ERROR

pushd "%SPT_WORKSPACE%"

@echo ### Check connection ###
call "%blhost%" %connect% -- get-property 1
if errorlevel 2 goto ERROR
@echo ### Flash erase. ###
call "%blhost%" %connect% fill-memory 0x2000F000 4 0xC0000004
call "%blhost%" %connect% configure-memory 0x09 0x2000F000
call "%blhost%" %connect% -t260000 -- flash-erase-all 0x09
if errorlevel 2 goto ERROR

@echo SUCCESS
    popd
	if not defined NO_PAUSE (pause)
	exit /b 0
:ERROR
    popd
    if not defined NO_PAUSE (pause)
    exit /b 2
