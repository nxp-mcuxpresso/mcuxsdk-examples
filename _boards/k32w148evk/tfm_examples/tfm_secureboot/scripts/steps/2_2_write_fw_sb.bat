@echo off
@rem -- The script for writing the OEM SB image. --

@rem -- Set variables. --
call ..\config.cmd
if errorlevel 1 goto ERROR

pushd "%SPT_WORKSPACE%"

@echo ### Check connection ###
call "%blhost%" %connect% -- get-property 1
if errorlevel 2 goto ERROR

@echo ### Update bootable image using SB file ###
@echo blhost %connect% -- receive-sb-file "bootable_images\tfm_secureboot.sb"
"%blhost%" %connect% -- receive-sb-file "bootable_images\tfm_secureboot.sb"

if errorlevel 2 goto ERROR

@echo SUCCESS
    popd
	if not defined NO_PAUSE (pause)
	exit /b 0
:ERROR
    popd
    if not defined NO_PAUSE (pause)
    exit /b 2
