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
            -fill 0xAB -maximum-addr "%TFM_EXAMPLE_NS%" -Intel 0x38000 ^
            -o input\tfm_secureboot.bin -Binary
if errorlevel 1 goto ERROR

@echo ### Create CFPA page - binary ###
@echo pfr generate-binary -c configs\cfpa.json -o configs\cfpa.bin
"%pfr%" generate-binary -c "configs\cfpa.json" -o "configs\cfpa.bin"
if errorlevel 1 goto ERROR

@echo ### Create CMPA page - binary ###
@echo pfr generate-binary -c configs\cmpa.json -e configs\mbi_config.json -o configs\cmpa.bin 
"%pfr%" generate-binary -c "configs\cmpa.json" -e "configs\mbi_config.json" -o "configs\cmpa.bin"
if errorlevel 1 goto ERROR

@echo ### Create bootable image ###
@echo nxpimage mbi export configs\mbi_config.json
"%nxpimage%" mbi export "configs\mbi_config.json"
if errorlevel 1 goto ERROR

if defined PROVISION_ONE_SB ( 
	@echo ### Create Provisioning SB file [CUST-MK-SK, CMPA, FW] ###
	@echo nxpdevhsm generate %connect% -k configs\cust_mk_sk.bin -o configs\sb_seed.bin -j configs\sb3_devhsm_config.json -f lpc55s3x bootable_images\dev_hsm_provisioning.sb
	"%nxpdevhsm%" generate %connect% -k "configs\cust_mk_sk.bin" -o "configs\sb_seed.bin" -j "configs\sb3_devhsm_config.json" -f lpc55s3x "bootable_images\dev_hsm_provisioning.sb"
	if errorlevel 1 goto ERROR
) else (
	@echo ### Create OEM SB file ###
	@echo nxpimage sb31 export configs\sb3_config.json
	"%nxpimage%" sb31 export "configs\sb3_config.json"
	if errorlevel 1 goto ERROR

	@echo ### Create Provisioning SB file [CUST-MK-SK] ###
	@echo nxpdevhsm generate %connect% -k configs\cust_mk_sk.bin -o configs\sb_seed.bin -f lpc55s3x bootable_images\dev_hsm_provisioning.sb
	@echo "%nxpdevhsm%"
	"%nxpdevhsm%" generate %connect% ^
		-k "configs\cust_mk_sk.bin" ^
		-o "configs\sb_seed.bin" ^
		-f lpc55s3x ^
		"bootable_images\dev_hsm_provisioning.sb"
	if errorlevel 1 goto ERROR
)

@echo SUCCESS
    popd
	if not defined NO_PAUSE (pause)
	exit /b 0
:ERROR
    popd
    if not defined NO_PAUSE (pause)
    exit /b 2
