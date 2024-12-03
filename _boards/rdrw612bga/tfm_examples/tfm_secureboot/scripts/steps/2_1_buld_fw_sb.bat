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


"%nxpimage%" utils binary-image extract -b "%TFM_EXAMPLE_S%" -a 0xC00 -s 0 input\tfm_secureboot_s_stripped.bin
xcopy /s /y %TFM_EXAMPLE_NS% input\
"%nxpimage%" utils binary-image merge -c "configs\combine_s_and_ns_images.yaml" input\tfm_secureboot.bin

@echo ###Check if file created - input\tfm_secureboot.bin  ###

if errorlevel 1 goto ERROR

REM @echo ### Create CFPA page - binary ###
REM @echo pfr generate-binary -c configs\cfpa.json -o configs\cfpa.bin
REM "%pfr%" generate-binary -c "configs\cfpa.json" -o "configs\cfpa.bin"
REM if errorlevel 1 goto ERROR

@echo ### Create bootable image ###
@echo nxpimage mbi export "configs\mbi_config.yaml"
"%nxpimage%" mbi export configs\mbi_config.yaml


if errorlevel 1 goto ERROR

@echo ### Create OEM SB file ###
@echo nxpimage sb31 export configs\sb3_config.yaml
"%nxpimage%" sb31 export "configs\sb3_config.yaml"
if errorlevel 1 goto ERROR


@echo SUCCESS
    popd
	if not defined NO_PAUSE (pause)
	exit /b 0
:ERROR
    popd
    if not defined NO_PAUSE (pause)
    exit /b 2
