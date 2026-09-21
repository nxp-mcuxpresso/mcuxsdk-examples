# Board-local override for RT2660 maestro_usb_speaker.
#
# The shared examples/audio_examples/maestro_usb_speaker/RT/streamer_pcm.c
# unconditionally reinitializes the RT10xx-family Audio PLL via
# CLOCK_InitAudioPll(clock_audio_pll_config_t), which does not exist on RT2660
# (RT2660 uses clock_cguana_avpll_config_t and the AUDIOPLL is already brought
# up by BOARD_InitBootClocks). Ship a board-local streamer_pcm.c here and only
# pull the shared streamer_pcm.h for the pcm_rtos_t type + API prototypes.

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
    SOURCES examples/audio_examples/maestro_usb_speaker/RT/streamer_pcm.h
)

mcux_add_include(
    BASE_PATH ${SdkRootDirPath}
    INCLUDES examples/audio_examples/maestro_usb_speaker/RT
)

mcux_add_macro(
    CC "-DUSB_STACK_FREERTOS\
        -DDEBUG_CONSOLE_TRANSFER_NON_BLOCKING\
        -DOSA_USED\
        -DSHELL_TASK_STACK_SIZE=4000"
)

# RT2660 runs example data (incl. the USB DMA descriptor structures) from
# cacheable PSRAM with the D-cache enabled. Mark the USB data section cacheable
# so the stack places its DMA data - the EHCI queue heads / DTDs / setup buffer
# and transfer buffers - in the NonCacheable (.ncache) region for coherent
# controller DMA. Without this the controller latches the first EP0 SETUP but
# the firmware reads a stale cached queue head, so enumeration never completes.
# (DATA_SECTION_IS_CACHEABLE uses the NonCacheable/.init sections the RT2660
# linker already provides; USB_DEVICE_CONFIG_BUFFER_PROPERTY_CACHEABLE would
# require a CacheLineData.init section that this linker script does not define.)
mcux_add_macro(
    CC "-DDATA_SECTION_IS_CACHEABLE=1"
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
# -- including the USB EHCI queue heads / DTDs / setup buffer and the USB DMA
# transfer buffers -- in the PSRAM window. PSRAM sits behind the XSPI
# last-level cache and is NOT in the USB controller's bus-master path, so the
# device controller cannot fetch its queue heads: the first EP0 SETUP is
# latched (ENDPTSETUPSTAT=1) but never serviced and enumeration never
# completes. Swap in the xspi_nor-based layout (shared with maestro_playback /
# maestro_record) that keeps all data -- and thus all DMA buffers -- in
# coherent internal SRAM/DTCM and relocates only the oversized FreeRTOS heap_4
# pool into PSRAM. Combined with DATA_SECTION_IS_CACHEABLE=1 above, the USB DMA
# structures land in the SRAM-resident NonCacheable (.ncache) region: reachable
# by the USB master and coherent. The xspi_nor_psram config is retained so the
# PSRAM boot XMCD still initializes the memory before the C runtime.
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
