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


"%nxpimage%" utils binary-image extract -b "%TFM_EXAMPLE_S%" -a 0xC00 -s 0 input\tfm_secureboot_s_stripped.bin
xcopy /s /y %TFM_EXAMPLE_NS% input\
"%nxpimage%" utils binary-image merge -c "configs\combine_s_and_ns_images.yaml" input\tfm_secureboot.bin

@echo ###Check if file created - input\tfm_secureboot.bin  ###

if errorlevel 1 goto ERROR


@echo ### Create bootable image ###
@echo nxpimage mbi export configs\mbi_config.yaml
"%nxpimage%" mbi export "configs\mbi_config.yaml"
if errorlevel 1 goto ERROR


::-w devhsm_debug_data
@echo ### Create Provisioning SB file [CUST-MK-SK, RKTH, SECURE_BOOT_EN] ###
@echo nxpdevhsm generate %connect% -k configs\key_rw61x.bin -o configs\sb_seed.bin -j configs\sb3_devhsm_config.yaml -f rw61x bootable_images\dev_hsm_provisioning.sb3
"%nxpdevhsm%" generate %connect% -k "configs\key_rw61x.bin" -o "configs\sb_seed.bin"  -j "configs\sb3_devhsm_config.yaml" -f rw61x "bootable_images\dev_hsm_provisioning.sb3"
if errorlevel 1 goto ERROR


@echo SUCCESS
    popd
	if not defined NO_PAUSE (pause)
	exit /b 0
:ERROR
    popd
    if not defined NO_PAUSE (pause)
    exit /b 2
