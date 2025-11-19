mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES examples/ota_examples/_common/sb3_api/sb3_api.h
            examples/ota_examples/_common/sb3_api/sb3_api_mcxn10.c
)

mcux_add_include(
    BASE_PATH ${SdkRootDirPath}
    INCLUDES examples/ota_examples/_common/sb3_api
)

mcux_add_mdk_configuration(
    LD "--diag_suppress=L6329W"
)

mcux_remove_armgcc_configuration(
    TARGETS debug
    CC "-g"
    CX "-g"
)

# Add or remove Linker File Configurations

mcux_remove_iar_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS debug release
    LINKER ${device_root}/${soc_portfolio}/${soc_series}/${device}/iar/${CONFIG_MCUX_TOOLCHAIN_LINKER_DEVICE_PREFIX}_flash.icf

)

mcux_remove_armgcc_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS debug release
    LINKER ${device_root}/${soc_portfolio}/${soc_series}/${device}/gcc/${CONFIG_MCUX_TOOLCHAIN_LINKER_DEVICE_PREFIX}_flash.ld
)

mcux_add_iar_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS debug release
    LINKER ${board_root}/${board}/ota_examples/ota_rom_basic/linker/MCXN236_flash.icf
)
mcux_add_armgcc_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS debug release
    LINKER ${board_root}/${board}/ota_examples/ota_rom_basic/linker/MCXN236_flash.ld
)
