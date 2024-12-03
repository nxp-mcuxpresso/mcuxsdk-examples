@echo off
@echo ********************
@echo *** PROVISIONING ***
@echo ********************

@rem - Disable pause after each step. -
set NO_PAUSE=n

pushd .\steps

@rem -- Set variables. --
call ..\config.cmd
if errorlevel 1 goto ERROR

@rem -- Flash Erase --
call 1_0_mass_erase.bat
@rem -- Build provision SB --
call 1_1_buld_provision_sb.bat
if errorlevel 2 goto ERROR

@rem -- Blow ROTKTH and SB3KDK --
@echo ### Provisioning the ROTKTH and SB3KDK is essential for further steps. ###
@echo ### So we will EXIT provisioning if this step is not selected. ###
@echo ### If the fuses are already provisioned say 'Y' to option to check if fuse values match. ###
call 1_2_seal_fuses.bat
if errorlevel 1 (

	goto ERROR
)
if errorlevel 2 goto ERROR

@rem -- Write IFR0 region --
call 1_3_seal_ifr_prince.bat
if errorlevel 2 goto ERROR
@rem -- Write provision SB --
call 1_4_write_provision_sb.bat
if errorlevel 2 goto ERROR
@rem -- Secure Seal --
call 1_5_seal_LC.bat
if errorlevel 2 goto ERROR


@echo *** Provisioning is SUCCESSFUL ***
    popd
	pause
	exit /b 0
:ERROR
	@echo *** Provisioning has FAILED! ***
    popd
    pause
    exit /b 2
