@echo off
@echo ********************
@echo **** FW UPDATE *****
@echo ********************

@rem - Disable pause after each step. -
set NO_PAUSE=y

pushd .\steps

@rem -- Build FW SB --
call 2_1_buld_fw_sb.bat
if errorlevel 2 goto ERROR
@rem -- Write FW SB  ---
call 2_2_write_fw_sb.bat
if errorlevel 2 goto ERROR

@echo *** FW Update is SUCCESSFUL ***
    popd
	pause
	exit /b 0
:ERROR
@echo *** FW Update is FAILED! ***
    popd
    pause
    exit /b 2
