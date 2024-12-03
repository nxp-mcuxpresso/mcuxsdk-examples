@echo off
@echo *********************
@echo ** Open Debug Port **
@echo *********************

@rem - Disable pause after each step -
set NO_PAUSE=y

pushd .\steps

@echo Life cycle has to be OEM Secure Closed/Closed for Debug authentication to function

@rem -- Open Debug Port --
call 3_1_open_debug_port.bat
if errorlevel 2 goto ERROR

@echo *** Debug Port Opening is SUCCESSFUL ***
    popd
	pause
	exit /b 0
:ERROR
@echo *** Debug Port Opening is FAILED! ***
    popd
    pause
    exit /b 2
