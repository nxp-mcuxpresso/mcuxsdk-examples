mcux_add_include(
    BASE_PATH ${SdkRootDirPath}
    INCLUDES ${board_root}/${board}/ota_examples/mcuboot_opensource
)

mcux_add_iar_configuration(
    CX "--diag_suppress=Pe257,Pa039"
)
mcux_add_mdk_configuration(
    LD "--diag_suppress=L6329W"
)


# Add or remove Linker File Configurations
mcux_remove_iar_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS debug release
    LINKER ${device_root}/MCX/MCXE/MCXE31B/iar/MCXE31B_flash.icf
)

mcux_remove_armgcc_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS debug release
    LINKER ${device_root}/MCX/MCXE/MCXE31B/gcc/MCXE31B_flash.ld
)

mcux_remove_mdk_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS debug release
    LINKER ${device_root}/MCX/MCXE/MCXE31B/arm/MCXE31B_flash.scf
)

mcux_add_iar_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS debug release
    LINKER ${board_root}/${board}/ota_examples/mcuboot_opensource/linker/MCXE31B_flash.icf
)
mcux_add_mdk_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS debug release
    LINKER ${board_root}/${board}/ota_examples/mcuboot_opensource/linker/MCXE31B_flash.scf
)
mcux_add_armgcc_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS debug release
    LINKER ${board_root}/${board}/ota_examples/mcuboot_opensource/linker/MCXE31B_flash.ld
)
