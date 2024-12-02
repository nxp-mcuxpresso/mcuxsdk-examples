@echo off
@rem -- The script for building of the Provisioning SB file. --

@rem -- Set variables. --
call ..\config.cmd
if errorlevel 1 goto ERROR

pushd "%SPT_WORKSPACE%"

@rem  -- Ping the device to establish communication. Required by nxpdevhsm. --
@echo ### Check connection ###
"%blhost%" %connect% -- get-property 1 0
if errorlevel 2 goto ERROR

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

@echo ### Create bootable image ###
@echo nxpimage mbi export configs\mbi_config.json
"%nxpimage%" mbi export "configs\mbi_config.json"
if errorlevel 1 goto ERROR

@echo ### Align bootable image size to PRINCE sub-region boundary ###
@echo %align% "bootable_images\tfm_secureboot.bin" 721408
call "%align%" "bootable_images\tfm_secureboot.bin" 721408
if errorlevel 1 goto ERROR

@echo ### Create IFR ROM configuration page - binary ###
@echo ifr generate-binary -d kw45xx -c "configs\romcfg.json" -o "configs\romcfg.bin"
"%ifr%" generate-binary -d kw45xx -c "configs\romcfg.json" ^
		-o "configs\romcfg.bin"
if errorlevel 1 goto ERROR

@echo ### Create SB file ###
"%nxpimage%" -v sb31 export "configs\sb3_config.json"  > tmp.txt 2>&1
if errorlevel 2 goto ERROR

@rem -- Parse and dump the ROTKTH obtained from nxpimage -v command above in a file --
@echo ### Dump ROTKTH in a file ###
for /F "delims=: tokens=3* " %%a in ('findstr  /ic:"ROTKTH" tmp.txt') do echo %%b > configs/rotkth.txt

del tmp.txt

@echo SUCCESS
    popd
	if not defined NO_PAUSE (pause)
	exit /b 0
:ERROR
    popd
    if not defined NO_PAUSE (pause)
    exit /b 2
