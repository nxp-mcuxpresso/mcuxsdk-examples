mcux_remove_armgcc_configuration(
  TARGETS release debug
  CC "-Os"
  CX "-Os"
)

mcux_add_armgcc_configuration(
  TARGETS release debug
  CC "-O3"
  CX "-O2"
)

mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES ${board_root}/${board}/eiq_examples/mpp/src/hardware_init.c
            ${board_root}/${board}/eiq_examples/mpp/src/pin_mux.c
            ${board_root}/${board}/eiq_examples/mpp/src/gpt_config.c
            ${board_root}/${board}/eiq_examples/mpp/inc/app.h
            ${board_root}/${board}/eiq_examples/mpp/inc/pin_mux.h
            # ${board_root}/${board}/eiq_examples/mpp/inc/dcd.h
)

mcux_add_macro(
    CC "-DFSL_SDK_ENABLE_DRIVER_CACHE_CONTROL=1"
)
mcux_add_macro(
    TOOLCHAINS armgcc
    TARGETS debug release
    CC "-DUSE_HYPERRAM\
       -DXIP_BOOT_HEADER_XMCD_ENABLE=1\
       -DDATA_SECTION_IS_CACHEABLE=1"
)
mcux_add_macro(
    TOOLCHAINS armgcc
    AS "-D__STARTUP_INITIALIZE_RAMFUNCTION"
)

mcux_add_armgcc_configuration(
  LD "-Xlinker --defsym=__heap_size__=0x8000\
      -Xlinker --defsym=__stack_size__=0x2000"
)

mcux_add_armgcc_linker_script(
    TARGETS debug release
    BASE_PATH ${SdkRootDirPath}/${board_root}/${board}/eiq_examples/mpp/src/
    LINKER MIMXRT1152xxxxx_flexspi_nor_hyperram.ld
)
