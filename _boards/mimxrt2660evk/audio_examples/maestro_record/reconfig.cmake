# Board-local override for RT2660 maestro_record.
#
# The shared examples/audio_examples/maestro_record/RT/streamer_pcm.c targets
# the RT10xx family (SAI1, DMAMUX, IOMUXC_GPR MCLK gate, PDM instance PDM).
# RT2660 uses AUDIO__SAI_0 and AUDIO__MICFIL on AUDIO__EDMA3 with MODCON MCLK
# gating (edma4 driver, no DMAMUX), so we ship a board-local streamer_pcm.c
# here and only pull the shared streamer_pcm.h for the pcm_rtos_t type + API
# prototypes.

mcux_add_include(
    BASE_PATH ${CMAKE_CURRENT_LIST_DIR}
    INCLUDES .
)

mcux_add_source(
    BASE_PATH ${CMAKE_CURRENT_LIST_DIR}
    SOURCES app_definitions.h
            streamer_pcm.c
            streamer_pcm.h
)

# NOTE: the shared examples/audio_examples/maestro_record/RT/streamer_pcm.h
# unconditionally includes fsl_dmamux.h, which does not exist on RT2660 (edma4
# uses EDMA_SetChannelMux rather than a separate DMAMUX peripheral). We
# therefore ship a board-local streamer_pcm.h in this directory and do NOT
# add the shared RT/ include path.

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
        --diag_suppress Pa050,Pa082,Pe188\
        --no_code_motion"
    CX "--no_code_motion"
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
# is shared with maestro_playback from audio_examples/common/linker.
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
