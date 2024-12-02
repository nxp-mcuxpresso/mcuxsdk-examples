#
# Copyright 2024 NXP
#
# SPDX-License-Identifier: BSD-3-Clause

cmake_minimum_required(VERSION 3.22.0)

include(${SdkRootDirPath}/cmake/extension/mcux.cmake)

project(wifi_ipv4_ipv6_echo LANGUAGES C CXX ASM PROJECT_BOARD_PORT_PATH examples/_boards/${board}/wifi_examples/wifi_ipv4_ipv6_echo)

include(${SdkRootDirPath}/CMakeLists.txt)
include(${SdkRootDirPath}/examples/lwip_examples/ipv4_ipv6_echo_common/CMakeLists.txt)

mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES
        examples/wifi_examples/wifi_ipv4_ipv6_echo/main.c

        examples/wifi_examples/wifi_ipv4_ipv6_echo/lwipopts.h
        examples/wifi_examples/wifi_ipv4_ipv6_echo/lwippools.h
        examples/wifi_examples/wifi_ipv4_ipv6_echo/wifi_config/wifi_config.h
        examples/wifi_examples/wifi_ipv4_ipv6_echo/FreeRTOSConfig.h

        examples/_boards/${board}/FreeRTOSConfigBoard.h
        examples/_boards/${board}/wifi_examples/common/app_config/app_config.h
        examples/_boards/${board}/wifi_examples/common/pin_mux.c
        examples/_boards/${board}/wifi_examples/common/pin_mux.h
)

mcux_add_include(
    BASE_PATH ${SdkRootDirPath}
    INCLUDES 
        examples/wifi_examples/wifi_ipv4_ipv6_echo
        examples/wifi_examples/wifi_ipv4_ipv6_echo/wifi_config
        examples/_boards/${board}/wifi_examples/common/app_config
        examples/_boards/${board}/wifi_examples/common
)

# Add additional configuration
mcux_add_macro(
    CC "-DUSE_RTOS=1\
       -DPRINTF_ADVANCED_ENABLE=1\
       -DSHELL_BUFFER_SIZE=160U"
)
mcux_add_iar_configuration(
	CC "--dlib_config full\
		--no_inline"
	CX "--no_clustering"
	LD "--semihosting"
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

# Linker files for wifi example optimize flash to RAM relocation when possible
# on given platform.
mcux_add_armgcc_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS debug release flexspi_nor_debug flexspi_nor_release flash_debug flash_release
    LINKER examples/_boards/${board}/wifi_examples/common/linker/${CONFIG_MCUX_TOOLCHAIN_LINKER_DEVICE_PREFIX}_wifi.ld
)

mcux_add_iar_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS debug release flexspi_nor_debug flexspi_nor_release flash_debug flash_release
    LINKER examples/_boards/${board}/wifi_examples/common/linker/${CONFIG_MCUX_TOOLCHAIN_LINKER_DEVICE_PREFIX}_wifi.icf
)
mcux_add_mdk_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS debug release flexspi_nor_debug flexspi_nor_release flash_debug flash_release
    LINKER examples/_boards/${board}/wifi_examples/common/linker/${CONFIG_MCUX_TOOLCHAIN_LINKER_DEVICE_PREFIX}_wifi.scf
)

include(${SdkRootDirPath}/examples/_boards/${board}/wifi_examples/wifi_ipv4_ipv6_echo/reconfig.cmake OPTIONAL)

mcux_convert_binary(BINARY ${APPLICATION_BINARY_DIR}/${MCUX_SDK_PROJECT_NAME}.bin)