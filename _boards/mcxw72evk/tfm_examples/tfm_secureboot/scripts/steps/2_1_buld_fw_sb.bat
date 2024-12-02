@echo off
@rem -- The script for building of the OEM SB file. --

@rem -- Set variables. --
call ..\config.cmd
if errorlevel 1 goto ERROR

pushd "%SPT_WORKSPACE%"

@echo ### Merge S and NS images to one input\tfm_secureboot.bin file ###
if not exist "input" (
    mkdir "input"
    if errorlevel 1 goto ERROR
)

if not exist "%TFM_EXAMPLE_S%" (
    echo [ERR] %TFM_EXAMPLE_S% is missed!
    goto ERROR
)

if not exist "%TFM_EXAMPLE_NS%" (
    echo [ERR] %TFM_EXAMPLE_NS% is missed!
    goto ERROR
)
"%sreccat%" "%TFM_EXAMPLE_S%" -Intel -offset - -minimum-addr "%TFM_EXAMPLE_S%" -Intel ^
            "%TFM_EXAMPLE_NS%" -Intel ^
            -fill 0xAB -maximum-addr "%TFM_EXAMPLE_NS%" -Intel 0xB0000 ^
            -o input\tfm_secureboot.bin -Binary
if errorlevel 1 goto ERROR

@echo ### Align bootable image size to PRINCE sub-region boundary ###
@echo %align% "bootable_images\tfm_secureboot.bin" 721408
call "%align%" "bootable_images\tfm_secureboot.bin" 721408
if errorlevel 1 goto ERROR

@echo ### Create bootable image ###
@echo nxpimage mbi export configs\mbi_config.json
"%nxpimage%" mbi export "configs\mbi_config.json"
if errorlevel 1 goto ERROR

@echo ### Create OEM SB file ###
@echo nxpimage sb31 export configs\sb3_config.json
"%nxpimage%" sb31 export "configs\sb3_config.json"
if errorlevel 1 goto ERROR


@echo SUCCESS
    popd
	if not defined NO_PAUSE (pause)
	exit /b 0
:ERROR
    popd
    if not defined NO_PAUSE (pause)
    exit /b 2
