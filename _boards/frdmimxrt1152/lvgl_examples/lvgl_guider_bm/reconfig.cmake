
mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES ${board_root}/${board}/lvgl_examples/hardware_init.c
            ${board_root}/${board}/lvgl_examples/app.h
)

mcux_add_include(
    BASE_PATH ${SdkRootDirPath}
    INCLUDES ${board_root}/${board}/lvgl_examples/lvgl_guider_bm
             ${board_root}/${board}/lvgl_examples
)

mcux_add_macro(
    CC "-DBOARD_USE_PCAL6524=1 -DSDK_I2C_BASED_COMPONENT_USED=1"
)

mcux_add_iar_configuration(
    LD "--config_def=__stack_size__=0x1800"
)
mcux_add_mdk_configuration(
    LD "--predefine=\"-D__stack_size__=0x1800\""
)
mcux_add_armgcc_configuration(
    LD "-Xlinker --defsym=__stack_size__=0x1800"
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
