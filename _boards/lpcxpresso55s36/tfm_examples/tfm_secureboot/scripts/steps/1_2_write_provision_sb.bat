@echo off
@rem -- The script for provisioning. --

@rem -- Set variables. --
call ..\config.cmd
if errorlevel 1 goto ERROR

pushd "%SPT_WORKSPACE%"

@echo ### Check connection ###
call "%blhost%" %connect% -- get-property 1
if errorlevel 2 goto ERROR

@echo ### Write Provisioning SB file ###
@echo blhost %connect% -- receive-sb-file bootable_images\dev_hsm_provisioning.sb
"%blhost%" %connect% -- receive-sb-file "bootable_images\dev_hsm_provisioning.sb
if errorlevel 2 goto ERROR

@echo ### Reset the processor, so the CUST_MK_SK key is applied ###
"%blhost%" %connect% -- reset
if errorlevel 2 goto ERROR

if not defined PROVISION_ONE_SB ( 
	@echo ### Write CFPA ###
	@echo blhost %connect% -- write-memory 0x0003DC00 configs\cfpa.bin
	"%blhost%" %connect% -- write-memory 0x0003DC00 "configs\cfpa.bin"
	if errorlevel 2 goto ERROR
	if errorlevel 1 (
	   @echo ### WARNING - CFPA page already written; check CFPA version to update the content ###
	)

	@echo ### Write CMPA ###
	@echo blhost %connect% -- write-memory 0x0003E200 configs\cmpa.bin
	"%blhost%" %connect% -- write-memory 0x0003E200 "configs\cmpa.bin"
	if errorlevel 2 goto ERROR

	@echo ### Write OEM SB file ###
	@echo blhost %connect% -- receive-sb-file "bootable_images\tfm_secureboot.sb"
	"%blhost%" %connect% -- receive-sb-file "bootable_images\tfm_secureboot.sb"
	if errorlevel 2 goto ERROR
)

@echo SUCCESS
    popd
	if not defined NO_PAUSE (pause)
	exit /b 0
:ERROR
    popd
    if not defined NO_PAUSE (pause)
    exit /b 2
