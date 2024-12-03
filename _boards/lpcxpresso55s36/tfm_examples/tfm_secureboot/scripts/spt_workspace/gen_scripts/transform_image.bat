@echo off
@rem Transform ELF image into SREC
pushd "C:\nxp\MCUX_Provi_v4\bin\tools\ide_utils\mcuxpresso\win"
call "arm-none-eabi-objcopy.exe" -O srec "C:\_ddm\KSDK\sdk_latest\mcu-sdk-2.0\boards\lpcxpresso55s36\tfm_examples\tfm_secureboot\tfm_secureboot_s\mdk\debug\tfm_secureboot_s.out" "C:\_ddm\KSDK\sdk_latest\mcu-sdk-2.0\boards\lpcxpresso55s36\tfm_examples\tfm_secureboot\mcux_sp_ws_crc\source_images\tfm_secureboot_s.srec"
popd
if errorlevel 1 exit 2