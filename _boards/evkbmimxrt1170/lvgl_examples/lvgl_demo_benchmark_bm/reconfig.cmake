
mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES ${board_root}/${board}/lvgl_examples/pin_mux.c
            ${board_root}/${board}/lvgl_examples/pin_mux.h
            ${board_root}/${board}/lvgl_examples/hardware_init.c
            ${board_root}/${board}/lvgl_examples/app.h
)

mcux_add_include(
    BASE_PATH ${SdkRootDirPath}
    INCLUDES ${board_root}/${board}/lvgl_examples/lvgl_demo_benchmark_bm
             ${board_root}/${board}/lvgl_examples
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
    TARGETS flexspi_nor_sdram
    LINKER ${device_root}/RT/RT1170/MIMXRT1176/iar/MIMXRT1176xxxxx_${core_id}_flexspi_nor_sdram.icf
)
mcux_remove_mdk_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS flexspi_nor_sdram
    LINKER ${device_root}/RT/RT1170/MIMXRT1176/arm/MIMXRT1176xxxxx_${core_id}_flexspi_nor_sdram.scf
)
mcux_remove_armgcc_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS flexspi_nor_sdram
    LINKER ${device_root}/RT/RT1170/MIMXRT1176/gcc/MIMXRT1176xxxxx_${core_id}_flexspi_nor_sdram.ld
)

# Add or remove Linker File Configurations
mcux_add_iar_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS flexspi_nor_sdram
    LINKER ${board_root}/${board}/lvgl_examples/linker/MIMXRT1176xxxxx_${core_id}_flexspi_nor_sdram.icf
)
mcux_add_mdk_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS flexspi_nor_sdram
    LINKER ${board_root}/${board}/lvgl_examples/linker/MIMXRT1176xxxxx_${core_id}_flexspi_nor_sdram.scf
)
mcux_add_armgcc_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS flexspi_nor_sdram
    LINKER ${board_root}/${board}/lvgl_examples/linker/MIMXRT1176xxxxx_${core_id}_flexspi_nor_sdram.ld
)
