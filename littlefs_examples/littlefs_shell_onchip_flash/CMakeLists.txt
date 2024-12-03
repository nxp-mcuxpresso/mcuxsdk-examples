
cmake_minimum_required(VERSION 3.22.0)

include(${SdkRootDirPath}/cmake/extension/mcux.cmake)

project(littlefs_shell_onchip_flash LANGUAGES C CXX ASM PROJECT_BOARD_PORT_PATH examples/_boards/${board}/littlefs_examples/littlefs_shell_onchip_flash)

include(${SdkRootDirPath}/CMakeLists.txt)

mcux_add_configuration(
    CC "-DLFS_NO_INTRINSICS=1\
        -DSDK_DEBUGCONSOLE_UART"
)

mcux_add_configuration(
    TARGETS release sdram_release hyperram_release flash_release
    CC "-DLFS_NO_ASSERT"
)

mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES middleware/littlefs/example/littlefs_shell/littlefs_shell.c
            examples/littlefs_examples/littlefs_shell/lfs_mflash.c
            examples/littlefs_examples/littlefs_shell/lfs_mflash.h
            examples/_boards/${board}/littlefs_examples/littlefs_shell_onchip_flash/peripherals.c
            examples/_boards/${board}/littlefs_examples/littlefs_shell_onchip_flash/peripherals.h
)

mcux_add_include(
    BASE_PATH ${SdkRootDirPath}
    INCLUDES middleware/littlefs/example/littlefs_shell
)

mcux_add_iar_configuration(
    LD "--config_def=__stack_size__=0x1000 --config_def=__heap_size__=0x1000"
)

mcux_add_mdk_configuration(
    LD "--predefine=\"-D__stack_size__=0x1000\" --predefine=\"-D__heap_size__=0x1000\""
)

mcux_add_armgcc_configuration(
    LD "-Xlinker --defsym=__stack_size__=0x1000 -Xlinker --defsym=__heap_size__=0x1000"
)

include(${SdkRootDirPath}/examples/_boards/${board}/littlefs_examples/littlefs_shell_onchip_flash/${core_id}/reconfig.cmake OPTIONAL)

mcux_convert_binary(BINARY ${APPLICATION_BINARY_DIR}/${MCUX_SDK_PROJECT_NAME}.bin)