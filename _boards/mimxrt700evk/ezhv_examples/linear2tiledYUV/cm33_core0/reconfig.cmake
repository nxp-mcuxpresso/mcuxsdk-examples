
mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES middleware/vglite/vglite_sdk/example/vglite_window/vglite_window.h
            middleware/vglite/vglite_sdk/example/vglite_window/vglite_window.c
            examples/_boards/${board}/FreeRTOSConfigBoard.h
)

mcux_add_include(
    BASE_PATH ${SdkRootDirPath}
    INCLUDES middleware/vglite/vglite_sdk/example/vglite_window
)

mcux_add_mdk_configuration(
LD "--library_type=microlib"
)

mcux_add_iar_configuration(
    CC "--diag_suppress Pa082,Pa050,Pa092"
    CX "--diag_suppress Pa082,Pa050,Pa092"
)

mcux_add_iar_configuration(
    TARGETS debug release flash_debug flash_release
    LD "--semihosting\
       --redirect __iar_sh_stdout=__iar_sh_stdout_swo"
)

mcux_add_macro(
    CC "-DGCID_REV_CID=gc555/0x423_ECO \
        -DCUSTOM_VGLITE_MEMORY_CONFIG=1 \
        -DVG_COMMAND_CALL=1 \
        -DVG_TARGET_FAST_CLEAR=0 \
        -DSDK_I2C_BASED_COMPONENT_USED=1 \
        -DSSD1963_DATA_WITDH=8 \
        -DSDK_OS_FREE_RTOS \
        -DBOARD_ENABLE_PSRAM_CACHE=0 \
        -DFLEXIO_MCULCD_DATA_BUS_WIDTH=8"
)

# add macro for format conversion
mcux_add_macro(
    # GPU_EN should be 1 if FORMAT_TYPE is 8
    CC "-DFORMAT_TYPE=2 \
        -DGPU_EN=1" 
)

# Remove default linker scripts. The app requires a custom set of linker scripts
mcux_remove_iar_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS debug release
    LINKER devices/RT/RT700/MIMXRT798S/iar/MIMXRT798Sxxxx_cm33_core0_ram.icf
)

mcux_remove_iar_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS flash_debug flash_release
    LINKER devices/RT/RT700/MIMXRT798S/iar/MIMXRT798Sxxxx_cm33_core0_flash.icf
)

mcux_remove_armgcc_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS debug release
    LINKER devices/RT/RT700/MIMXRT798S/gcc/MIMXRT798Sxxxx_cm33_core0_ram.ld
)

mcux_remove_armgcc_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS flash_debug flash_release
    LINKER devices/RT/RT700/MIMXRT798S/gcc/MIMXRT798Sxxxx_cm33_core0_flash.ld
)

mcux_remove_mdk_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS debug release
    LINKER devices/RT/RT700/MIMXRT798S/arm/MIMXRT798Sxxxx_cm33_core0_ram.scf
)

mcux_remove_mdk_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS flash_debug flash_release
    LINKER devices/RT/RT700/MIMXRT798S/arm/MIMXRT798Sxxxx_cm33_core0_flash.scf
)

# Add custom linker scripts
mcux_add_iar_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS debug release
    LINKER examples/_boards/${board}/ezhv_examples/linker/MIMXRT798Sxxxx_cm33_core0_ram.icf
)

mcux_add_iar_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS flash_debug flash_release
    LINKER examples/_boards/${board}/ezhv_examples/linker/MIMXRT798Sxxxx_cm33_core0_flash.icf
)

mcux_add_armgcc_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS debug release
    LINKER examples/_boards/${board}/ezhv_examples/linker/MIMXRT798Sxxxx_cm33_core0_ram.ld
)

mcux_add_armgcc_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS flash_debug flash_release
    LINKER examples/_boards/${board}/ezhv_examples/linker/MIMXRT798Sxxxx_cm33_core0_flash.ld
)

mcux_add_mdk_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS debug release
    LINKER examples/_boards/${board}/ezhv_examples/linker/MIMXRT798Sxxxx_cm33_core0_ram.scf
)

mcux_add_mdk_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS flash_debug flash_release
    LINKER examples/_boards/${board}/ezhv_examples/linker/MIMXRT798Sxxxx_cm33_core0_flash.scf
)
