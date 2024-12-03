cmake_minimum_required(VERSION 3.22.0)

include(${SdkRootDirPath}/cmake/extension/mcux.cmake)

project(ota_mcuboot_client_wifi LANGUAGES C CXX ASM PROJECT_BOARD_PORT_PATH examples/_boards/${board}/ota_examples/ota_mcuboot_client/wifi)

include(${SdkRootDirPath}/CMakeLists.txt)
mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES examples/ota_examples/ota_mcuboot_client/ota_mcuboot_client.c
            examples/ota_examples/ota_mcuboot_client/flash_helper.h
            examples/ota_examples/ota_mcuboot_client/flash_helper.c
            examples/ota_examples/ota_mcuboot_client/FreeRTOSConfig.h
            examples/_boards/${board}/FreeRTOSConfigBoard.h
            examples/ota_examples/ota_mcuboot_client/httpsclient.c
            examples/ota_examples/ota_mcuboot_client/httpsclient.h
            examples/ota_examples/ota_mcuboot_client/ota_http.c
            examples/ota_examples/ota_mcuboot_client/ota_http.h
            examples/ota_examples/ota_mcuboot_client/ota_config.h
            examples/ota_examples/ota_mcuboot_client/core_http_config.c
            examples/ota_examples/ota_mcuboot_client/core_http_config.h
            examples/ota_examples/ota_mcuboot_client/network_wifi_nxp/network_cfg.h
            examples/ota_examples/ota_mcuboot_client/network_wifi_nxp/init_wifi.c
            examples/ota_examples/ota_mcuboot_client/network_wifi_nxp/lwipopts.h
            examples/ota_examples/ota_mcuboot_client/network_wifi_nxp/wifi_config.h
)


mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES examples/_boards/${board}/wifi_examples/common/app_config/app_config.h
            examples/_boards/${board}/ota_examples/mcuboot_opensource/flash_partitioning/flash_partitioning.h
            examples/_boards/${board}/ota_examples/mcuboot_opensource/flash_partitioning/flash_partitioning.c
            examples/_boards/${board}/ota_examples/mcuboot_opensource/sblconfig.h
)

mcux_add_include(
    BASE_PATH ${SdkRootDirPath}
    INCLUDES examples/ota_examples/ota_mcuboot_client/network_wifi_nxp
             examples/ota_examples/ota_mcuboot_client
             examples/_boards/${board}/ota_examples/ota_mcuboot_client/wifi
             examples/_boards/${board}/wifi_examples/common/app_config
             examples/_boards/${board}/ota_examples/mcuboot_opensource/flash_partitioning
             examples/_boards/${board}/ota_examples/mcuboot_opensource
             examples/_boards/${board}
)


mcux_remove_armgcc_configuration(
    TARGETS debug
    CC "-g"
    CX "-g"
)
mcux_add_armgcc_configuration(
    TARGETS debug
    CC "-g1"
    CX "-g1"
)

mcux_remove_mdk_configuration(
    TARGETS flexspi_nor_release
    CC "-Oz"
)
mcux_add_mdk_configuration(
    TARGETS flexspi_nor_release
    CC "-Os"
)

mcux_add_mcux_configuration(
    TARGETS debug
    CC "-Og"
    CX "-g3"
)


mcux_add_macro(
    CC "-DPRINTF_ADVANCED_ENABLE=1\
       -DMCUBOOT_APPLICATION=1\
       -DUSE_RTOS=1\
       -DLWIP_DNS=1\
       -DLWIP_DHCP=1\
       -DENET_RXBUFF_NUM=14\
       -DSHELL_BUFFER_SIZE=128U\
       -DSHELL_TASK_PRIORITY=1"
)

mcux_add_macro(
    TOOLCHAINS armgcc
    CC "-D_POSIX_SOURCE"
)


mcux_add_iar_configuration(
    LD "--config_def=__stack_size__=0x2000 --config_def=__heap_size__=0x2000"
)

mcux_add_mdk_configuration(
    LD "--predefine=\"-D__stack_size__=0x2000\" --predefine=\"-D__heap_size__=0x2000\""
)

mcux_add_armgcc_configuration(
    LD "-Xlinker --defsym=__stack_size__=0x2000 -Xlinker --defsym=__heap_size__=0x2000"
)

include(${SdkRootDirPath}/examples/_boards/${board}/ota_examples/ota_mcuboot_client/wifi/reconfig.cmake OPTIONAL)

mcux_convert_binary(BINARY ${APPLICATION_BINARY_DIR}/${MCUX_SDK_PROJECT_NAME}.bin)
