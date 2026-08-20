mcux_remove_armgcc_configuration(
  TARGETS flexspi_nor_hyperram_debug flexspi_nor_hyperram_release
  CC "-Os"
  CX "-Os"
)

mcux_add_armgcc_configuration(
  TARGETS flexspi_nor_hyperram_debug flexspi_nor_hyperram_release
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
)

mcux_add_macro(
    CC "-DFSL_SDK_ENABLE_DRIVER_CACHE_CONTROL=1 \
        -DUSB_HOST_CONFIG_BUFFER_PROPERTY_CACHEABLE=1"
    CX "-DFSL_SDK_ENABLE_DRIVER_CACHE_CONTROL=1 \
        -DUSB_HOST_CONFIG_BUFFER_PROPERTY_CACHEABLE=1"
)

mcux_add_macro(
    TOOLCHAINS armgcc
    TARGETS flexspi_nor_hyperram_debug flexspi_nor_hyperram_release
    CC "-DXIP_BOOT_HEADER_XMCD_ENABLE=1 \
        -DDATA_SECTION_IS_CACHEABLE=1"
)

mcux_add_macro(
    TOOLCHAINS armgcc
    AS "-D__STARTUP_INITIALIZE_RAMFUNCTION"
)

mcux_remove_armgcc_linker_script(
    TARGETS flexspi_nor_hyperram_debug flexspi_nor_hyperram_release
    BASE_PATH ${SdkRootDirPath}
    LINKER ${device_root}/${soc_portfolio}/${soc_series}/${device}/gcc/${CONFIG_MCUX_TOOLCHAIN_LINKER_DEVICE_PREFIX}_flexspi_nor_hyperram.ld
)

mcux_remove_armgcc_linker_script(
    TARGETS flexspi_nor_debug flexspi_nor_release
    BASE_PATH ${SdkRootDirPath}
    LINKER ${device_root}/${soc_portfolio}/${soc_series}/${device}/gcc/${CONFIG_MCUX_TOOLCHAIN_LINKER_DEVICE_PREFIX}_flexspi_nor.ld
)

mcux_add_armgcc_linker_script(
    TARGETS flexspi_nor_hyperram_debug flexspi_nor_hyperram_release flexspi_nor_debug flexspi_nor_release
    BASE_PATH ${SdkRootDirPath}/${board_root}/${board}/eiq_examples/mpp/linker_files
    LINKER MIMXRT1152xxxxx_flexspi_nor_hyperram.ld
)
