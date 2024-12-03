

# Add additional configuration
mcux_add_macro(
    AS "-DIMG_BAUDRATE=96000000"
)

mcux_add_macro(
    TOOLCHAINS armgcc iar mdk
    AS "-DXIP_IMAGE"
    CC "-DXIP_IMAGE"
)

mcux_add_linker_symbol(
	SYMBOLS "XIP_IMAGE=1"
)

mcux_add_iar_configuration(
    LD "--redirect _Printf=_PrintfFull"
    CC "--dlib_config full"
)
mcux_add_armgcc_configuration(
    TARGETS debug
    CC "-O1"
)


# Remove additional configuration
mcux_remove_armgcc_configuration(
    TARGETS debug
    CC "-O0"
)
mcux_remove_armgcc_configuration(
    LD "--specs=nano.specs"
)

mcux_remove_armgcc_linker_script(
    BASE_PATH ${SdkRootDirPath}
    LINKER devices/LPC/LPC54000/LPC54S018M/gcc/LPC54S018J4M_spifi_flash.ld
)
mcux_remove_iar_linker_script(
    BASE_PATH ${SdkRootDirPath}
    LINKER devices/LPC/LPC54000/LPC54S018M/iar/LPC54S018J4M_spifi_flash.icf
)
mcux_remove_mdk_linker_script(
    BASE_PATH ${SdkRootDirPath}
    LINKER devices/LPC/LPC54000/LPC54S018M/arm/LPC54S018J4M_spifi_flash.scf
)


# Add or remove Linker File Configurations
mcux_add_iar_linker_script(
    BASE_PATH ${SdkRootDirPath}
    LINKER examples/_boards/${board}/aws_examples/common/linker/LPC54S018M_spifi_flash.icf
)
mcux_add_mdk_linker_script(
    BASE_PATH ${SdkRootDirPath}
    LINKER examples/_boards/${board}/aws_examples/common/linker/LPC54S018M_spifi_flash.scf
)
mcux_add_armgcc_linker_script(
    BASE_PATH ${SdkRootDirPath}
    LINKER examples/_boards/${board}/aws_examples/common/linker/LPC54S018M_spifi_flash.ld
)
