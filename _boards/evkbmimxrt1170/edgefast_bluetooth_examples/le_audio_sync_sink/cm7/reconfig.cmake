mcux_add_source(
  BASE_PATH ${SdkRootDirPath}
  SOURCES
    middleware/edgefast_bluetooth/boards/${board}/edgefast_bluetooth_examples/le_audio_sync_sink/${core_id}/hardware_init.c
    middleware/edgefast_bluetooth/boards/${board}/edgefast_bluetooth_examples/le_audio_sync_sink/pin_mux.c
    middleware/edgefast_bluetooth/boards/${board}/edgefast_bluetooth_examples/le_audio_sync_sink/pin_mux.h
)
mcux_add_include(
  BASE_PATH ${SdkRootDirPath}
  INCLUDES
    middleware/edgefast_bluetooth/boards/${board}/edgefast_bluetooth_examples/le_audio_sync_sink
    middleware/edgefast_bluetooth/boards/${board}/edgefast_bluetooth_examples/le_audio_sync_sink/${core_id}
)
mcux_add_macro(
  CC "-DPRINTF_FLOAT_ENABLE=1\
      -DSCANF_ADVANCED_ENABLE=0\
      -DSCANF_FLOAT_ENABLE=0"
)

mcux_add_macro(
  TARGETS ram_0x1400_debug
  TOOLCHAINS iar
)

mcux_add_macro(
  AS "-DNDEBUG"
  CC "-DNDEBUG"
  CX "-DNDEBUG"
  TARGETS ram_0x1400_release
  TOOLCHAINS iar
)

mcux_add_macro(
  CC "-DUSE_SDRAM"
  TARGETS sdram_txt_debug
  TOOLCHAINS iar
)

mcux_add_macro(
  AS "-DNDEBUG"
  CC "-DNDEBUG\
      -DUSE_SDRAM"
  CX "-DNDEBUG"
  TARGETS sdram_txt_release
  TOOLCHAINS iar
)

mcux_add_macro(
  TOOLCHAINS mdk
)

mcux_add_macro(
  TARGETS ram_0x1400_debug
  TOOLCHAINS mdk
)

mcux_add_macro(
  AS "-DNDEBUG"
  CC "-DNDEBUG"
  CX "-DNDEBUG"
  TARGETS ram_0x1400_release
  TOOLCHAINS mdk
)

mcux_add_macro(
  CC "-DUSE_SDRAM"
  TARGETS sdram_txt_debug
  TOOLCHAINS mdk
)

mcux_add_macro(
  CC "-DUSE_SDRAM"
  TARGETS sdram_txt_release
  TOOLCHAINS mdk
)

mcux_add_macro(
  TOOLCHAINS armgcc
)

mcux_add_macro(
  AS "-D__STARTUP_CLEAR_BSS"
  CC "-DUSE_SDRAM"
  TARGETS sdram_txt_debug
  TOOLCHAINS armgcc
)

mcux_add_macro(
  AS "-D__STARTUP_CLEAR_BSS"
  CC "-DUSE_SDRAM"
  TARGETS sdram_txt_release
  TOOLCHAINS armgcc
)
mcux_add_iar_configuration(
    TARGETS ram_0x1400_debug
    AS "-r"
    CC "-On\
       --debug\
       --no_cse\
       --no_unroll\
       --no_inline\
       --no_code_motion\
       --no_tbaa\
       --no_clustering\
       --no_scheduling"
    CX "--debug\
       --no_cse\
       --no_unroll\
       --no_code_motion\
       --no_tbaa\
       --no_clustering\
       --no_scheduling"
)
mcux_add_iar_configuration(
    TARGETS sdram_txt_debug
    AS "-r"
    CC "-Om\
       --debug\
       --no_cse\
       --no_unroll\
       --no_inline\
       --no_code_motion\
       --no_tbaa\
       --no_clustering\
       --no_scheduling"
    CX "--debug\
       --no_cse\
       --no_unroll\
       --no_code_motion\
       --no_tbaa\
       --no_clustering\
       --no_scheduling"
)
mcux_add_iar_configuration(
    TARGETS sdram_txt_release
    CC "-Om"
)
mcux_add_mdk_configuration(
    AS "--cpu Cortex-M7.dfp"
    CC "--cpu Cortex-M7.dfp"
    CX "--cpu Cortex-M7.dfp"
    LD "--cpu Cortex-M7.dfp\
       --predefine="-D__stack_size__=0x4000"\
       --predefine="-D__heap_size__=0x4000""
)
mcux_add_mdk_configuration(
    TARGETS ram_0x1400_debug
    CC "-g"
)
mcux_add_mdk_configuration(
    TARGETS ram_0x1400_release
    CC "-O3"
)
mcux_add_mdk_configuration(
    TARGETS sdram_txt_debug
    CC "-b\
       --split_sections\
       --diag_suppress=66,1296,186\
       --c99\
       --library_interface=armcc\
       -g"
    CX "-b\
       --split_sections\
       --diag_suppress=66"
    LD "--remove\
       --entry=Reset_Handler\
       --map\
       --xref\
       --callgraph\
       --symbols\
       --info sizes\
       --info totals\
       --info unused\
       --info veneers\
       --strict"
)
mcux_add_mdk_configuration(
    TARGETS sdram_txt_release
    CC "-b\
       --split_sections\
       --diag_suppress=66,1296,186\
       --c99\
       --library_interface=armcc\
       -O3"
    CX "-b\
       --split_sections\
       --diag_suppress=66"
    LD "--remove\
       --entry=Reset_Handler\
       --map\
       --xref\
       --callgraph\
       --symbols\
       --info sizes\
       --info totals\
       --info unused\
       --info veneers\
       --strict"
)
mcux_add_armgcc_configuration(
    AS "-mfloat-abi=hard\
       -mthumb"
    CC "-Wall\
       -mfloat-abi=hard\
       -mthumb\
       -MMD\
       -MP"
    CX "-Wall\
       -mfloat-abi=hard\
       -mthumb\
       -MMD\
       -MP"
    LD "-Wall\
       -mfloat-abi=hard"
)
mcux_add_armgcc_configuration(
    TARGETS sdram_txt_debug
    CC "-g\
       -O0"
    CX "-g\
       -O0"
    LD "-g"
)
mcux_add_armgcc_configuration(
    TARGETS sdram_txt_release
    CC "-Os"
    CX "-Os"
)
mcux_add_iar_linker_script(
  BASE_PATH ${SdkRootDirPath}
  LINKER examples/_boards/${board}/edgefast_bluetooth_examples/le_audio_sync_sink/linker/MIMXRT1176xxxxx_cm7_ram_0x1400.icf
  TARGETS
    ram_0x1400_debug
    ram_0x1400_release
)
mcux_add_mdk_linker_script(
  BASE_PATH ${SdkRootDirPath}
  LINKER examples/_boards/${board}/edgefast_bluetooth_examples/le_audio_sync_sink/linker/MIMXRT1176xxxxx_cm7_ram_0x1400.scf
  TARGETS
    ram_0x1400_debug
    ram_0x1400_release
)
