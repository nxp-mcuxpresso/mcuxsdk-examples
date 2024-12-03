cmake_minimum_required(VERSION 3.22.0)

include(${SdkRootDirPath}/cmake/extension/mcux.cmake)

project(ota_rom_server_enet LANGUAGES C CXX ASM PROJECT_BOARD_PORT_PATH examples/_boards/${board}/ota_examples/ota_rom_server/enet)

include(${SdkRootDirPath}/CMakeLists.txt)
mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES examples/ota_examples/ota_rom_server/ota_rom_server.c
            examples/_boards/${board}/ota_examples/ota_rom_server/enet/network_cfg.h
            examples/ota_examples/ota_rom_server/network_enet/init_enet.c
            examples/ota_examples/ota_rom_server/network_enet/lwipopts.h
            examples/ota_examples/ota_rom_server/httpsrv_multipart.c
            examples/ota_examples/ota_rom_server/httpsrv_multipart.h
            examples/ota_examples/ota_rom_server/httpsrv_fs_data.c
            examples/ota_examples/ota_rom_server/FreeRTOSConfig.h
            examples/_boards/${board}/FreeRTOSConfigBoard.h
)

mcux_add_include(
    BASE_PATH ${SdkRootDirPath}
    INCLUDES examples/_boards/${board}/ota_examples/ota_rom_server/enet/${core_id}
             examples/ota_examples/ota_rom_server/network_enet
             examples/_boards/${board}/ota_examples/ota_rom_server
             examples/_boards/${board}/ota_examples/ota_rom_server/enet
             examples/_boards/${board}
)

mcux_add_include(
    BASE_PATH ${SdkRootDirPath}
    INCLUDES examples/ota_examples/ota_rom_server
)

mcux_add_armgcc_configuration(
    TARGETS debug
    CC "-g1"
    CX "-g1"
)
mcux_add_mcux_configuration(
    TARGETS debug
    CC "-Og"
    CX "-g3"
)

mcux_add_macro(
    CC "-DUSE_RTOS=1\
       -DPRINTF_ADVANCED_ENABLE=1\
       -DHTTPSRV_CFG_WEBSOCKET_ENABLED=0"
)
mcux_add_macro(
    TOOLCHAINS armgcc
    CC "-D_POSIX_SOURCE"
)

mcux_remove_armgcc_configuration(
    TARGETS debug
    CC "-g"
    CX "-g"
)

mcux_add_iar_configuration(
    LD "--config_def=__stack_size__=0x1000 --config_def=__heap_size__=0x10000"
)

mcux_add_mdk_configuration(
    LD "--predefine=\"-D__stack_size__=0x1000\" --predefine=\"-D__heap_size__=0x10000\""
)

mcux_add_armgcc_configuration(
    LD "-Xlinker --defsym=__stack_size__=0x1000 -Xlinker --defsym=__heap_size__=0x10000"
)


include(${SdkRootDirPath}/examples/_boards/${board}/ota_examples/ota_rom_server/enet/reconfig.cmake OPTIONAL)

mcux_convert_binary(BINARY ${APPLICATION_BINARY_DIR}/${MCUX_SDK_PROJECT_NAME}.bin)
