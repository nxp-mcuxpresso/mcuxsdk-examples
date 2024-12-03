@echo off
@echo ********************
@echo *** CFPA UPDATE ****
@echo ********************

@rem - Disable pause after each step -
set NO_PAUSE=y

pushd .\steps

@rem -- Build CFPA SB --
call 3_1_buld_cfpa_sb.bat
if errorlevel 2 goto ERROR
@rem -- Write CFPA SB --
call 3_2_write_cfpa_sb.bat
if errorlevel 2 goto ERROR

@echo *** CFPA Update is SUCCESSFUL ***
    popd
	pause
	exit /b 0
:ERROR
@echo *** CFPA Update is FAILED! ***
    popd
    pause
    exit /b 2
