
mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES ${board_root}/${board}/lvgl_examples/pin_mux.c
            ${board_root}/${board}/lvgl_examples/pin_mux.h
            ${board_root}/${board}/lvgl_examples/hardware_init.c
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
    TARGETS flexspi_nor_sdram_debug flexspi_nor_sdram_release
    LD "--semihosting\
       --redirect __iar_sh_stdout=__iar_sh_stdout_swo"
)

mcux_add_macro(
    CC "-DGCID_REV_CID=gc355/0x0_1216 \
        -DCUSTOM_VGLITE_MEMORY_CONFIG=0"
)

# Add or remove Linker File Configurations
mcux_remove_iar_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS flexspi_nor_sdram
    LINKER ${device_root}/RT/RT1160/MIMXRT1166/iar/MIMXRT1166xxxxx_${core_id}_flexspi_nor_sdram.icf
)
mcux_remove_mdk_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS flexspi_nor_sdram
    LINKER ${device_root}/RT/RT1160/MIMXRT1166/arm/MIMXRT1166xxxxx_${core_id}_flexspi_nor_sdram.scf
)
mcux_remove_armgcc_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS flexspi_nor_sdram
    LINKER ${device_root}/RT/RT1160/MIMXRT1166/gcc/MIMXRT1166xxxxx_${core_id}_flexspi_nor_sdram.ld
)

# Add or remove Linker File Configurations
mcux_add_iar_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS flexspi_nor_sdram
    LINKER ${board_root}/${board}/lvgl_examples/linker/MIMXRT1166xxxxx_${core_id}_flexspi_nor_sdram.icf
)
mcux_add_mdk_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS flexspi_nor_sdram
    LINKER ${board_root}/${board}/lvgl_examples/linker/MIMXRT1166xxxxx_${core_id}_flexspi_nor_sdram.scf
)
mcux_add_armgcc_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS flexspi_nor_sdram
    LINKER ${board_root}/${board}/lvgl_examples/linker/MIMXRT1166xxxxx_${core_id}_flexspi_nor_sdram.ld
)
