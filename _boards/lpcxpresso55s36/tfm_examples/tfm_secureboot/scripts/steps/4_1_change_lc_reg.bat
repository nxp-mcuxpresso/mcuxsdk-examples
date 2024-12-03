@echo off
@rem - The script to open debug port -

@rem -- Set variables --
call ..\config.cmd
if errorlevel 1 goto ERROR

pushd "%SPT_WORKSPACE%"

call "%nxpdebugmbox%" test-connection
if errorlevel 1 goto ERROR

@echo Current Life-Cycle register state:
call "%nxpdebugmbox%" read-memory -h 0x4002006C 4

@echo ### Change Life-Cycle register to 0xF ###
call "%nxpdebugmbox%" write-memory 0x4002006c "{{0f 00 00 00}}"


@echo SUCCESS
    popd
	if not defined NO_PAUSE (pause)
	exit /b 0
:ERROR
    popd
    if not defined NO_PAUSE (pause)
    exit /b 2

