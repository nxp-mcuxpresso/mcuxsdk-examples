# Board-local override for RT2660 maestro_playback.
#
# The shared examples/audio_examples/maestro_playback/RT/streamer_pcm.c targets
# the RT10xx family (SAI1, DMAMUX, IOMUXC_GPR MCLK gate). RT2660 uses
# AUDIO__SAI_0 on AUDIO__EDMA3 with MODCON MCLK gating, so we ship a board-local
# streamer_pcm.c here and only pull the shared streamer_pcm.h for the
# pcm_rtos_t type + API prototypes.

mcux_add_include(
    BASE_PATH ${CMAKE_CURRENT_LIST_DIR}
    INCLUDES .
)

mcux_add_source(
    BASE_PATH ${CMAKE_CURRENT_LIST_DIR}
    SOURCES app_definitions.h
            streamer_pcm.c
)

# Header-only pull from the shared RT/ folder (declares pcm_rtos_t and API).
mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES examples/audio_examples/maestro_playback/RT/streamer_pcm.h
)

mcux_add_include(
    BASE_PATH ${SdkRootDirPath}
    INCLUDES examples/audio_examples/maestro_playback/RT
)

mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES ${board_root}/${board}/sdmmc_config.c
            ${board_root}/${board}/sdmmc_config.h
)

mcux_add_macro(
    CC "-DDEBUG_CONSOLE_TRANSFER_NON_BLOCKING\
        -DOSA_USED\
        -DSHELL_TASK_STACK_SIZE=4000\
        -DBOARD_USE_PCAL6524=1\
        -DBOARD_USE_PCA9555=1\
        -DSDK_I2C_BASED_COMPONENT_USED=1"
)

mcux_add_iar_configuration(
    CC "--vla\
        --align_sp_on_irq\
        --macro_positions_in_diagnostics\
        --header_context\
        --no_unroll\
        --no_inline\
        --no_tbaa\
        --no_unaligned_access\
        --dlib_config full\
        --diag_suppress Pa050,Pa082,Pe188"
)

mcux_add_macro(
    TOOLCHAINS armgcc mcux iar
    CC "-DPLATFORM_ID=LVM_IMXRT2660"
)

# ---------------------------- Linker override ----------------------------- #
# The stock device linker (MIMXRT2663xxxxx_xspi_nor_psram.ld) places .data/.bss
# -- including every uSDHC ADMA-visible SD/FatFs buffer -- in the PSRAM
# non-cacheable window, which is NOT coherent with the uSDHC ADMA bus master
# (the XSPI last-level cache is bypassed by the master), so `file list` / mount
# hang. Swap in an xspi_nor-based layout that keeps all data (and thus all DMA
# buffers) in coherent internal SRAM/DTCM and relocates only the oversized
# FreeRTOS heap_4 pool into PSRAM. The xspi_nor_psram config is retained so the
# PSRAM boot XMCD still initializes the memory before the C runtime. The linker
# is shared with maestro_record from audio_examples/common/linker.
mcux_remove_armgcc_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS xspi_nor_psram_debug xspi_nor_psram_release
    LINKER devices/RT/RT2660/MIMXRT2663/gcc/MIMXRT2663xxxxx_xspi_nor_psram.ld
)
mcux_add_armgcc_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS xspi_nor_psram_debug xspi_nor_psram_release
    LINKER ${board_root}/${board}/audio_examples/common/linker/MIMXRT2663xxxxx_xspi_nor_heap_psram.ld
)
