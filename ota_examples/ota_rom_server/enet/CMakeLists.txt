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
    INCLUDES boards/_boards/${board}/ota_examples/ota_rom_server/enet/${core_id}
             examples/ota_examples/ota_rom_server/network_enet
             examples/_boards/${board}/ota_examples/ota_rom_server
             examples/_boards/${board}/ota_examples/ota_rom_server/enet
             examples/_boards/${board}
)


mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES examples/ota_examples/ota_mcuboot_server/webpage/httpsrv.css
            examples/ota_examples/ota_mcuboot_server/webpage/index.html
            examples/ota_examples/ota_mcuboot_server/webpage/ota_reboot.html
            examples/ota_examples/ota_mcuboot_server/webpage/welcome.html
            examples/ota_examples/ota_mcuboot_server/webpage/favicon.ico
            examples/ota_examples/ota_mcuboot_server/webpage/request.js
            examples/ota_examples/ota_mcuboot_server/webpage/NXP_logo.png
            examples/ota_examples/ota_mcuboot_server/webpage/ota.shtml
            middleware/mcuboot_opensource/boot/nxp_mcux_sdk/app_support/mcuboot_app_support.c
            middleware/mcuboot_opensource/boot/nxp_mcux_sdk/app_support/mcuboot_app_support.h
            examples/_boards/${board}/ota_examples/mcuboot_opensource/flash_partitioning/flash_partitioning.h
            examples/_boards/${board}/ota_examples/mcuboot_opensource/flash_partitioning/flash_partitioning.c
            examples/_boards/${board}/ota_examples/mcuboot_opensource/sblconfig.h
            middleware/mcuboot_opensource/boot/nxp_mcux_sdk/include/mcuboot_config/mcuboot_config.h
            middleware/mcuboot_opensource/boot/nxp_mcux_sdk/include/mcuboot_config/mcuboot_logging.h
            middleware/mcuboot_opensource/boot/nxp_mcux_sdk/include/flash_map.h
            middleware/mcuboot_opensource/boot/nxp_mcux_sdk/include/sysflash/sysflash.h
)

mcux_add_include(
    BASE_PATH ${SdkRootDirPath}
    INCLUDES examples/ota_examples/ota_rom_server
             middleware/mcuboot_opensource/boot/nxp_mcux_sdk/app_support
             middleware/mcuboot_opensource/boot/nxp_mcux_sdk
             middleware/mcuboot_opensource/boot/nxp_mcux_sdk/include
             middleware/mcuboot_opensource/boot/nxp_mcux_sdk/include/mcuboot_config
             middleware/mcuboot_opensource/boot/nxp_mcux_sdk/include/sysflash
             examples/_boards/${board}/ota_examples/mcuboot_opensource
             examples/_boards/${board}/ota_examples/mcuboot_opensource/flash_partitioning
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
