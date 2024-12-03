@echo off
@echo ********************
@echo *** PROVISIONING ***
@echo ********************

@rem - Disable pause after each step. -
set NO_PAUSE=y

pushd .\steps

@rem -- Flash Erase --
call 1_0_flash_erase.bat
@rem -- Build provision SB --
call 1_1_buld_provision_sb.bat
if errorlevel 2 goto ERROR
@rem -- Write provision SB --
call 1_2_write_provision_sb.bat
if errorlevel 2 goto ERROR
@rem -- Secure Seal --
call 1_3_seal_irreversible.bat
if errorlevel 2 goto ERROR

@echo *** Provisioning is SUCCESSFUL ***
    popd
	pause
	exit /b 0
:ERROR
@echo *** Provisioning is FAILED! ***
    popd
    pause
    exit /b 2
