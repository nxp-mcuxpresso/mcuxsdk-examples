
#
# Copyright 2026 NXP
#
# SPDX-License-Identifier: BSD-3-Clause

mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES examples/_boards/${board}/wifi_examples/common/host_sleep.c
           examples/_boards/${board}/wifi_examples/common/host_sleep.h
           examples/_boards/${board}/wifi_examples/common/pin_mux.c
           examples/_boards/${board}/wifi_examples/common/pin_mux.h
           examples/_boards/${board}/wifi_examples/wifi_cli/lwip_config/lwipopts.h
           examples/_boards/${board}/wifi_examples/wifi_cli/lwip_config/lwippools.h
           examples/_boards/${board}/wifi_examples/wifi_cli/lwip_config/lwiphooks.h
           examples/_boards/${board}/wifi_examples/wifi_cli/freertos_config/FreeRTOSConfig.h
           examples/_boards/${board}/wifi_examples/wifi_cli/wifi_config/wifi_config.h
           examples/_boards/${board}/wifi_examples/common/hardware_init.c
           examples/_boards/${board}/wifi_examples/common/app.h
           examples/_boards/${board}/sdmmc_config.c
           examples/_boards/${board}/sdmmc_config.h
           examples/_boards/${board}/wifi_bt_config.c
           examples/_boards/${board}/wifi_bt_config.h
           examples/_boards/${board}/FreeRTOSConfigBoard.h
)

mcux_add_include(
    BASE_PATH ${SdkRootDirPath}
    INCLUDES examples/_boards/${board}/wifi_examples/common
            examples/_boards/${board}/wifi_examples/wifi_cli
            examples/_boards/${board}/wifi_examples/wifi_cli/wifi_config
            examples/_boards/${board}/wifi_examples/wifi_cli/lwip_config
            examples/_boards/${board}/wifi_examples/wifi_cli/freertos_config
)


mcux_add_mdk_configuration(
    CC "-mfloat-abi=hard"
)

mcux_add_iar_configuration(
	CC "--dlib_config full\
		--no_inline"
	CX "--no_clustering"
	LD "--semihosting"
)

mcux_add_macro(
    CC "-DFSL_SDK_ENABLE_DRIVER_CACHE_CONTROL=1\
       -DBOARD_NETWORK_USE_100M_ENET_PORT=1\
       -DUSE_RTOS=1\
       -DPRINTF_ADVANCED_ENABLE=1\
       -DSDMMCHOST_ENABLE_CACHE_LINE_ALIGN_TRANSFER=1\
       -DFSL_USDHC_ENABLE_SCATTER_GATHER_TRANSFER=1\
       -DBOARD_USE_PCAL6524=1\
       -DSDK_I2C_BASED_COMPONENT_USED=1\
       -DCONFIG_TX_RX_ZERO_COPY=1"
)
mcux_add_macro(
    TOOLCHAINS armgcc
    TARGETS flexspi_nor_release flexspi_nor_debug
    AS "-D__STARTUP_INITIALIZE_RAMFUNCTION"
)

# Add or remove Linker File Configurations
mcux_remove_armgcc_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS flexspi_nor_debug flexspi_nor_release
    LINKER devices/RT/RT1150/MIMXRT1152/gcc/MIMXRT1152xxxxx_flexspi_nor.ld
)
mcux_remove_iar_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS flexspi_nor_debug flexspi_nor_release
    LINKER devices/RT/RT1150/MIMXRT1152/iar/MIMXRT1152xxxxx_flexspi_nor.icf
)
mcux_remove_mdk_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS flexspi_nor_debug flexspi_nor_release
    LINKER devices/RT/RT1150/MIMXRT1152/arm/MIMXRT1152xxxxx_flexspi_nor.scf
)

# Add or remove Linker File Configurations
mcux_add_armgcc_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS flexspi_nor_debug flexspi_nor_release
    LINKER examples/_boards/${board}/wifi_examples/common/linker/MIMXRT1152xxxxx_flexspi_nor.ld
)
mcux_add_iar_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS flexspi_nor_debug flexspi_nor_release
    LINKER examples/_boards/${board}/wifi_examples/common/linker/MIMXRT1152xxxxx_flexspi_nor.icf
)
mcux_add_mdk_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS flexspi_nor_debug flexspi_nor_release
    LINKER examples/_boards/${board}/wifi_examples/common/linker/MIMXRT1152xxxxx_flexspi_nor.scf
)

mcux_add_iar_configuration(
    LD "--config_def=__stack_size__=0x400\
        --config_def=__heap_size__=0x400"
)
mcux_add_mdk_configuration(
    LD "--predefine=\"-D__stack_size__=0x400\"\
        --predefine=\"-D__heap_size__=0x400\""
)
mcux_add_armgcc_configuration(
    LD "-Xlinker --defsym=__stack_size__=0x400\
        -Xlinker --defsym=__heap_size__=0x400"
)
