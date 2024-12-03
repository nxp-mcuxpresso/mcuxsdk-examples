@echo off
@echo *********************
@echo ** Open Debug Port **
@echo *********************

@rem - Disable pause after each step -
set NO_PAUSE=y

pushd .\steps

@rem -- Change the life-cycle register (if OTP life-cycle is 0x303), otherwise disable the next call --
call 3_1_change_lc_reg.bat
if errorlevel 2 goto ERROR

@rem -- Open Debug Port --
call 3_2_open_debug_port.bat
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
