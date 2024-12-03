@echo off
@rem -- The script for writing of the OEM SB file. --

@rem -- Set variables. --
call ..\config.cmd
if errorlevel 1 goto ERROR

pushd "%SPT_WORKSPACE%"

@echo ### Check connection ###
call "%blhost%" %connect% -- get-property 1
if errorlevel 2 goto ERROR

REM @echo ### Write Customer Field Programmable Area [CFPA] ###
REM @echo blhost %connect% -- write-memory 0x0003DC00 configs\cfpa.bin
REM "%blhost%" %connect% -- write-memory 0x0003DC00 "configs\cfpa.bin"
REM if errorlevel 2 goto ERROR
REM if errorlevel 1 (
        REM @echo ### Probably, CFPA_PAGE_VERSION is low. ###
REM )

@echo ### Update Customer Field Programmable Area [CFPA] ###
@echo blhost %connect% -- receive-sb-file "bootable_images\tfm_secureboot_cfpa.sb"
"%blhost%" %connect% -- receive-sb-file "bootable_images\tfm_secureboot_cfpa.sb"

if errorlevel 2 goto ERROR
if errorlevel 1 (
    @echo ### Probably, CFPA_PAGE_VERSION is low. ###
	goto ERROR
)

@echo SUCCESS
    popd
	if not defined NO_PAUSE (pause)
	exit /b 0
:ERROR
    popd
    if not defined NO_PAUSE (pause)
    exit /b 2
