mcux_add_include(
    BASE_PATH ${SdkRootDirPath}
    INCLUDES ${board_root}/${board}/ota_examples/mcuboot_opensource
)

mcux_add_iar_configuration(
    CX "--diag_suppress=Pe257,Pa039"
)
mcux_add_iar_configuration(
    TARGETS debug
    CC "-Om"
)
mcux_add_mdk_configuration(
    TARGETS debug
    CC "-O1\
       -flto"
)
mcux_add_mdk_configuration(
    LD "--diag_suppress=L6329W"
)
mcux_add_armgcc_configuration(
    TARGETS debug
    CC "-O1"
)



# Remove additional configuration
mcux_remove_iar_configuration(
    TARGETS debug
    CC "-On"
)
mcux_remove_mdk_configuration(
    TARGETS debug
    CC "-O0"
)
mcux_remove_armgcc_configuration(
    TARGETS debug
    CC "-O0"
)


# Add or remove Linker File Configurations
mcux_remove_iar_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS debug release
    LINKER ${device_root}/MCX/MCXA/MCXA366/iar/MCXA366_flash.icf
)

mcux_remove_armgcc_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS debug release
    LINKER ${device_root}/MCX/MCXA/MCXA366/gcc/MCXA366_flash.ld
)

mcux_remove_mdk_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS debug release
    LINKER ${device_root}/MCX/MCXA/MCXA366/arm/MCXA366_flash.scf
)

mcux_add_iar_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS debug release
    LINKER ${board_root}/${board}/ota_examples/mcuboot_opensource/linker/MCXA366_flash.icf
)
mcux_add_mdk_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS debug release
    LINKER ${board_root}/${board}/ota_examples/mcuboot_opensource/linker/MCXA366_flash.scf
)
mcux_add_armgcc_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS debug release
    LINKER ${board_root}/${board}/ota_examples/mcuboot_opensource/linker/MCXA366_flash.ld
)
