
mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES examples/_boards/${board}/ota_examples/ota_mcuboot_client/enet/mbedtls_user_config.h
)


mcux_add_include(
    BASE_PATH ${SdkRootDirPath}
    INCLUDES examples/ota_examples/ota_mcuboot_client
)

mcux_add_mdk_configuration(
    LD "--diag_suppress=L6329W"
)

mcux_add_macro(
    CC "-DMBEDTLS_USER_CONFIG_FILE=\\\"mbedtls_user_config.h\\\"\
        -DMBEDTLS_CONFIG_FILE=\\\"els_pkc_mbedtls_config.h\\\""
)
mcux_remove_mdk_configuration(
    TARGETS flexspi_nor_release
    CC "-Oz"
)
mcux_remove_armgcc_configuration(
    TARGETS debug
    CC "-g"
    CX "-g"
)

# Add or remove Linker File Configurations
mcux_remove_iar_linker_script(
        BASE_PATH ${SdkRootDirPath}
        LINKER devices/MCX/MCXN/MCXN947/iar/MCXN947_cm33_core0_flash.icf
)

mcux_remove_armgcc_linker_script(
        BASE_PATH ${SdkRootDirPath}
        LINKER devices/MCX/MCXN/MCXN947/gcc/MCXN947_cm33_core0_flash.ld
)

mcux_remove_mdk_linker_script(
        BASE_PATH ${SdkRootDirPath}
        LINKER devices/MCX/MCXN/MCXN947/arm/MCXN947_cm33_core0_flash.scf
)

mcux_add_iar_linker_script(
    BASE_PATH ${SdkRootDirPath}
    LINKER examples/_boards/${board}/ota_examples/linker_common_ifr/mcxn10_cm33_flash.icf
)
mcux_add_mdk_linker_script(
    BASE_PATH ${SdkRootDirPath}
    LINKER examples/_boards/${board}/ota_examples/linker_common_ifr/mcxn10_cm33_flash.scf
)
mcux_add_armgcc_linker_script(
    BASE_PATH ${SdkRootDirPath}
    LINKER examples/_boards/${board}/ota_examples/linker_common_ifr/mcxn10_cm33_flash.ld
)
