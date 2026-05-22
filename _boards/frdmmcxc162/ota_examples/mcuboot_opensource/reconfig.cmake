mcux_add_include(
    BASE_PATH ${SdkRootDirPath}
    INCLUDES ${board_root}/${board}/ota_examples/mcuboot_opensource
)

# Add or remove Linker File Configurations
mcux_remove_iar_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS debug release
    LINKER ${device_root}/MCX/MCXC/MCXC162/iar/MCXC162_flash.icf
)
mcux_remove_armgcc_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS debug release
    LINKER ${device_root}/MCX/MCXC/MCXC162/gcc/MCXC162_flash.ld
)

mcux_add_iar_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS debug release
    LINKER ${board_root}/${board}/ota_examples/mcuboot_opensource/linker/MCXC162_flash.icf
)

mcux_add_armgcc_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS debug release
    LINKER ${board_root}/${board}/ota_examples/mcuboot_opensource/linker/MCXC162_flash.ld
)

mcux_remove_armgcc_configuration(
    TARGETS debug
    CC "-O0"
)

mcux_add_configuration(
    TARGETS debug
    CC "-Os"
    TOOLCHAINS armgcc
)
