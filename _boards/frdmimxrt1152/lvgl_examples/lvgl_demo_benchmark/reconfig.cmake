
mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES ${board_root}/${board}/lvgl_examples/hardware_init.c
            ${board_root}/${board}/lvgl_examples/app.h
)

mcux_add_include(
    BASE_PATH ${SdkRootDirPath}
    INCLUDES ${board_root}/${board}/lvgl_examples/lvgl_demo_benchmark
             ${board_root}/${board}/lvgl_examples
)

mcux_add_iar_configuration(
    CC "--diag_suppress Pa082,Pa050,Pa092"
    CX "--diag_suppress Pa082,Pa050,Pa092"
)

mcux_add_iar_configuration(
    TARGETS flexspi_nor_hyperram_debug flexspi_nor_hyperram_release
    LD "--semihosting\
       --redirect __iar_sh_stdout=__iar_sh_stdout_swo"
)

mcux_add_macro(
    CC "-DGCID_REV_CID=gc355/0x0_1216 \
        -DCUSTOM_VGLITE_MEMORY_CONFIG=0 \
        -DBOARD_USE_PCAL6524=1 \
        -DSDK_I2C_BASED_COMPONENT_USED=1"
)

# Add or remove Linker File Configurations
mcux_remove_iar_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS flexspi_nor_hyperram_debug flexspi_nor_hyperram_release
    LINKER ${device_root}/RT/RT1150/MIMXRT1152/iar/MIMXRT1152xxxxx_flexspi_nor_hyperram.icf
)
mcux_remove_mdk_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS flexspi_nor_hyperram_debug flexspi_nor_hyperram_release
    LINKER ${device_root}/RT/RT1150/MIMXRT1152/arm/MIMXRT1152xxxxx_flexspi_nor_hyperram.scf
)
mcux_remove_armgcc_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS flexspi_nor_hyperram_debug flexspi_nor_hyperram_release
    LINKER ${device_root}/RT/RT1150/MIMXRT1152/gcc/MIMXRT1152xxxxx_flexspi_nor_hyperram.ld
)

# Add or remove Linker File Configurations
mcux_add_iar_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS flexspi_nor_hyperram_debug flexspi_nor_hyperram_release
    LINKER ${board_root}/${board}/lvgl_examples/linker/MIMXRT1152xxxxx_flexspi_nor_hyperram.icf
)
mcux_add_mdk_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS flexspi_nor_hyperram_debug flexspi_nor_hyperram_release
    LINKER ${board_root}/${board}/lvgl_examples/linker/MIMXRT1152xxxxx_flexspi_nor_hyperram.scf
)
mcux_add_armgcc_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS flexspi_nor_hyperram_debug flexspi_nor_hyperram_release
    LINKER ${board_root}/${board}/lvgl_examples/linker/MIMXRT1152xxxxx_flexspi_nor_hyperram.ld
)
