mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES examples/_boards/${board}/ota_examples/mcuboot_opensource/mbedtls_user_config.h
)

mcux_add_include(
    BASE_PATH ${SdkRootDirPath}
    INCLUDES examples/_boards/${board}/ota_examples/mcuboot_opensource
)

# Add additional configuration
mcux_add_macro(
    CC "-DMBEDTLS_USER_CONFIG_FILE=\\\"mbedtls_user_config.h\\\""
)
mcux_add_iar_configuration(
    TARGETS debug
    CC "-Om"
)
mcux_add_iar_configuration(
    TARGETS debug release
    CC "--diag_suppress=Pe550"
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
    CC "-Og"
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
    LINKER devices/MCX/MCXA/MCXA153/iar/MCXA153_flash.icf
)

mcux_remove_armgcc_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS debug release
    LINKER devices/MCX/MCXA/MCXA153/gcc/MCXA153_flash.ld
)

mcux_remove_mdk_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS debug release
    LINKER devices/MCX/MCXA/MCXA153/arm/MCXA153_flash.scf
)

mcux_add_iar_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS debug release
    LINKER examples/_boards/${board}/ota_examples/mcuboot_opensource/linker/MCXA153_flash.icf
)
mcux_add_mdk_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS debug release
    LINKER examples/_boards/${board}/ota_examples/mcuboot_opensource/linker/MCXA153_flash.scf
)
mcux_add_armgcc_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS debug release
    LINKER examples/_boards/${board}/ota_examples/mcuboot_opensource/linker/MCXA153_flash.ld
)
