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
::"%blhost%" %connect%  -t 60000 -- flash-erase-region 0x08000000 0x200000

@echo "%blhost%" %connect% -t240000 -- receive-sb-file "bootable_images\tfm_secureboot.sb3"
"%blhost%" %connect% -t240000 -- receive-sb-file "bootable_images\tfm_secureboot.sb3"

if errorlevel 2 goto ERROR

@echo SUCCESS
    popd
	if not defined NO_PAUSE (pause)
	exit /b 0
:ERROR
    popd
    if not defined NO_PAUSE (pause)
    exit /b 2
