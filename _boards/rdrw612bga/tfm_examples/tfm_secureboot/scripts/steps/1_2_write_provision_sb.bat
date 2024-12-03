@echo off
@rem -- The script for provisioning. --

@rem -- Set variables. --
call ..\config.cmd
if errorlevel 1 goto ERROR

pushd "%SPT_WORKSPACE%"

choice /C NY /T 10 /D N /M "WARNING: Do you want to change the CUST_SK_MK, RKTH and SECURE_BOOT_EN OTPs in Device (irreversible operation) "
if %ERRORLEVEL% neq 2 goto EXIT

@echo ### Check connection ###
call "%blhost%" %connect% -- get-property 1
if errorlevel 2 goto ERROR

@echo ### Write Provisioning SB file ###
@echo blhost %connect% -t 300000 -- receive-sb-file bootable_images\dev_hsm_provisioning.sb3
"%blhost%" %connect% -t 300000 -- receive-sb-file "bootable_images\dev_hsm_provisioning.sb3"
if errorlevel 2 goto ERROR

@echo ### Reset the processor, so the CUST_MK_SK key, RKTH and Secure-Boot-En is applied ###
"%blhost%" %connect% -- reset
if errorlevel 2 goto ERROR


@echo SUCCESS
:EXIT
    popd
	if not defined NO_PAUSE (pause)
	exit /b 0
:ERROR
    popd
    if not defined NO_PAUSE (pause)
    exit /b 2
