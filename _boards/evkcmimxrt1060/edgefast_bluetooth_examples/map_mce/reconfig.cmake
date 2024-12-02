

mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES middleware/edgefast_bluetooth/boards/${board}/edgefast_bluetooth_examples/configs/FreeRTOSConfig.h
            examples/_boards/${board}/FreeRTOSConfigBoard.h
            middleware/edgefast_bluetooth/output/templates/config/app_config.h
#           middleware/edgefast_bluetooth/boards/${board}/edgefast_bluetooth_examples/map_mce/app_bluetooth_config.h
            examples/_boards/${board}/sdmmc_config.c
            examples/_boards/${board}/sdmmc_config.h
            examples/_boards/${board}/wifi_bt_config.c
            examples/_boards/${board}/wifi_bt_config.h
            middleware/edgefast_bluetooth/boards/${board}/edgefast_bluetooth_examples/map_mce/hardware_init.c
            middleware/edgefast_bluetooth/boards/${board}/edgefast_bluetooth_examples/map_mce/pin_mux.c
            middleware/edgefast_bluetooth/boards/${board}/edgefast_bluetooth_examples/map_mce/pin_mux.h
            middleware/edgefast_bluetooth/source/impl/ethermind/controller/configs/mbedtls/mbedtls_config_client.h
            components/wifi_bt_module/incl/wifi_bt_module_config.h
)


mcux_add_include(
    BASE_PATH ${SdkRootDirPath}
    INCLUDES middleware/edgefast_bluetooth/boards/${board}/edgefast_bluetooth_examples/configs
             middleware/edgefast_bluetooth/boards/${board}/edgefast_bluetooth_examples/map_mce
             middleware/edgefast_bluetooth/output/templates/config
             middleware/edgefast_bluetooth/source/impl/ethermind/controller/configs/mbedtls
             #  middleware/edgefast_bluetooth/include
             components/wifi_bt_module/incl/
)




# Add additional configuration
mcux_add_macro(
    TOOLCHAINS iar
    CC "-DFILE=void"
)
mcux_add_macro(
    TOOLCHAINS iar
    TARGETS flexspi_nor_release
    CC "-DLFS_NO_ASSERT"
)
mcux_add_mdk_configuration(
    LD "--keep=*(._bt_*)\
       --keep=*(._net_buf_pool*)\
       --keep=*(._settings_handler_static*)\
       --keep=mflash_drv.o(*)\
       --diag_suppress=6329\
       --diag_suppress=6319\
       --diag_suppress=6675\
       --diag_suppress=6775\
       --legacyalign\
       --diag_suppress=3912"
)
mcux_add_macro(
    CC "-DSDIO_ENABLED=1\
       -DHAL_UART_DMA_ENABLE=1\
       -DHAL_AUDIO_DMA_INIT_ENABLE=0\
       -DLFS_NO_INTRINSICS=1\
       -DLFS_NO_ERROR=1\
       -DCONFIG_ARM=1\
       -DEDGEFAST_BT_LITTLEFS_MFLASH\
       -DFSL_SDK_ENABLE_DRIVER_CACHE_CONTROL=1\
       -DGATT_DB\
       -DFSL_DRIVER_TRANSFER_DOUBLE_WEAK_IRQ=0\
       -DFSL_FEATURE_FLASH_PAGE_SIZE_BYTES=4096\
       -DUSB_HOST_CONFIG_BUFFER_PROPERTY_CACHEABLE=1"
)
mcux_add_macro(
    TOOLCHAINS armgcc
    TARGETS flexspi_nor_release
    CC "-DLFS_NO_ASSERT"
)


# Add or remove Linker File Configurations
mcux_remove_iar_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS flexspi_nor_debug flexspi_nor_release
    LINKER devices/RT/RT1060/MIMXRT1062/iar/MIMXRT1062xxxxx_flexspi_nor.icf
)
mcux_remove_mdk_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS flexspi_nor_debug flexspi_nor_release
    LINKER devices/RT/RT1060/MIMXRT1062/arm/MIMXRT1062xxxxx_flexspi_nor.scf
)
mcux_remove_armgcc_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS flexspi_nor_debug flexspi_nor_release
    LINKER devices/RT/RT1060/MIMXRT1062/gcc/MIMXRT1062xxxxx_flexspi_nor.ld
)


# Add or remove Linker File Configurations
mcux_add_iar_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS flexspi_nor_debug flexspi_nor_release
    LINKER middleware/edgefast_bluetooth/boards/${board}/edgefast_bluetooth_examples/map_mce/linker/iar/MIMXRT1062xxxxx_flexspi_nor.icf
)
mcux_add_mdk_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS flexspi_nor_debug flexspi_nor_release
    LINKER middleware/edgefast_bluetooth/boards/${board}/edgefast_bluetooth_examples/map_mce/linker/arm/MIMXRT1062xxxxx_flexspi_nor.scf
)
mcux_add_armgcc_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS flexspi_nor_debug flexspi_nor_release
    LINKER middleware/edgefast_bluetooth/boards/${board}/edgefast_bluetooth_examples/map_mce/linker/gcc/MIMXRT1062xxxxx_flexspi_nor.ld
)
