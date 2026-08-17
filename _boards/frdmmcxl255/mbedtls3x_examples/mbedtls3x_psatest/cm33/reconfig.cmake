# ARMGCC debug, IAR debug and release target does not fit; slightly increase optimization as workaround
mcux_remove_armgcc_configuration(
    TARGETS debug
    CC "-O0"
    CX "-O0"
)

mcux_add_armgcc_configuration(
    TARGETS debug
    CC "-Os"
    CX "-Os"
)

mcux_remove_iar_configuration(
    TARGETS debug
    CC "-On"
    CX "-On"
)

mcux_remove_iar_configuration(
    TARGETS release
    CC "-Oh"
    CX "-Oh"
)

mcux_add_iar_configuration(
    TARGETS debug release
    CC "-Ohz"
    CX "-Ohz"
)

# The device default IAR linker reserves 32 KB of flash (0x5E000-0x65FFF) for a
# secondary (core1) image. mbedtls3x_psatest is a single-core CM33 application
# and ships no core1 image, so reclaim that flash for .text.
mcux_remove_iar_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS debug release
    LINKER ${device_root}/MCX/MCXL/MCXL255/iar/MCXL255_cm33_flash.icf
)

mcux_add_iar_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS debug release
    LINKER ${board_root}/${board}/mbedtls3x_examples/mbedtls3x_psatest/cm33/linkers/MCXL255_cm33_flash.icf
)

mcux_remove_mdk_configuration(
    TARGETS debug
    CC "-O1"
    CX "-O1"
)

mcux_add_mdk_configuration(
    TARGETS debug
    CC "-Oz"
    CX "-Oz"
)
