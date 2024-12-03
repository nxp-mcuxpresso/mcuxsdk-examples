@echo off
@echo ********************
@echo * FLASH MASS ERASE *
@echo ********************

@rem - Disable pause after each step. -
set NO_PAUSE=y

pushd .\steps

@rem -- Flash Mass Erase --
call 1_0_flash_erase.bat
if errorlevel 2 goto ERROR

@echo *** Flash mass erase is SUCCESSFUL ***
    popd
	pause
	exit /b 0
:ERROR
@echo *** Flash mass erase is FAILED! ***
    popd
    pause
    exit /b 2
