
cmake_minimum_required(VERSION 3.22.0)

include(${SdkRootDirPath}/cmake/extension/mcux.cmake)

project(pbap_pse LANGUAGES C CXX ASM PROJECT_BOARD_PORT_PATH examples/_boards/${board}/edgefast_bluetooth_examples/pbap_pse)

mcux_add_include(
 	BASE_PATH ${SdkRootDirPath}
    INCLUDES middleware/edgefast_bluetooth/boards/${board}/edgefast_bluetooth_examples/pbap_pse
	TARGET edgefast_bluetooth_app.h
)
mcux_add_source(
	BASE_PATH ${SdkRootDirPath}
    SOURCES middleware/edgefast_bluetooth/boards/${board}/edgefast_bluetooth_examples/pbap_pse/${multicore_foldername}/edgefast_bluetooth_app.h
    CONFIG True
    PREINCLUDE True
)

include(${SdkRootDirPath}/CMakeLists.txt)
mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES middleware/edgefast_bluetooth/examples/pbap_pse/main.c
            middleware/edgefast_bluetooth/examples/pbap_pse/app_pbap_pse.c
            middleware/edgefast_bluetooth/examples/pbap_pse/app_pbap_pse.h
            middleware/edgefast_bluetooth/examples/pbap_pse/app_connect.c
            middleware/edgefast_bluetooth/examples/pbap_pse/app_connect.h
)

mcux_add_include(
    BASE_PATH ${SdkRootDirPath}
    INCLUDES middleware/edgefast_bluetooth/examples/pbap_pse
)

mcux_add_macro(
    CC "-DUSE_RTOS=1\
       -DPRINTF_ADVANCED_ENABLE=1\
       -DSDK_OS_FREE_RTOS\
       -DSDK_DEBUGCONSOLE_UART=1\
       -DSHELL_USE_COMMON_TASK=0\
       -DSHELL_TASK_STACK_SIZE=2048\
       -DSHELL_TASK_PRIORITY=configMAX_PRIORITIES-2\
       -DLPUART_RING_BUFFER_SIZE=1024U"
)

mcux_add_iar_configuration(
    LD "--semihosting"
)

mcux_add_mdk_configuration(
    LD "--library_type=microlib"
)

include(${SdkRootDirPath}/examples/_boards/${board}/edgefast_bluetooth_examples/pbap_pse/${multicore_foldername}/reconfig.cmake OPTIONAL)

mcux_convert_binary(BINARY ${APPLICATION_BINARY_DIR}/${MCUX_SDK_PROJECT_NAME}.bin)