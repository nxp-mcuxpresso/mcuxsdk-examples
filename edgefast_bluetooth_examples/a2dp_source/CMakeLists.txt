
cmake_minimum_required(VERSION 3.22.0)

include(${SdkRootDirPath}/cmake/extension/mcux.cmake)

project(a2dp_source LANGUAGES C CXX ASM PROJECT_BOARD_PORT_PATH examples/_boards/${board}/edgefast_bluetooth_examples/a2dp_source)

mcux_add_include(
 	BASE_PATH ${SdkRootDirPath}
    INCLUDES middleware/edgefast_bluetooth/boards/${board}/edgefast_bluetooth_examples/a2dp_source
	TARGET edgefast_bluetooth_app.h
)
mcux_add_source(
	BASE_PATH ${SdkRootDirPath}
    SOURCES middleware/edgefast_bluetooth/boards/${board}/edgefast_bluetooth_examples/a2dp_source/${multicore_foldername}/edgefast_bluetooth_app.h
    CONFIG True
    PREINCLUDE True
)

include(${SdkRootDirPath}/CMakeLists.txt)
mcux_add_source(
  BASE_PATH ${SdkRootDirPath}
  SOURCES
    middleware/edgefast_bluetooth/examples/a2dp_source/a2dp_pl_media_48KHz.h
    middleware/edgefast_bluetooth/examples/a2dp_source/app_a2dp_source.c
    middleware/edgefast_bluetooth/examples/a2dp_source/app_a2dp_source.h
    middleware/edgefast_bluetooth/examples/a2dp_source/app_connect.c
    middleware/edgefast_bluetooth/examples/a2dp_source/app_connect.h
    middleware/edgefast_bluetooth/examples/a2dp_source/app_discover.c
    middleware/edgefast_bluetooth/examples/a2dp_source/app_discover.h
    middleware/edgefast_bluetooth/examples/a2dp_source/app_shell.c
    middleware/edgefast_bluetooth/examples/a2dp_source/app_shell.h
    middleware/edgefast_bluetooth/examples/a2dp_source/main.c
)

mcux_add_include(
  BASE_PATH ${SdkRootDirPath}
  INCLUDES middleware/edgefast_bluetooth/examples/a2dp_source
)

mcux_add_macro(
  CC "-DLPUART_RING_BUFFER_SIZE=1024U\
      -DPRINTF_ADVANCED_ENABLE=1\
      -DSDK_DEBUGCONSOLE_UART=1\
      -DSDK_OS_FREE_RTOS\
      -DSHELL_TASK_PRIORITY=configMAX_PRIORITIES-2\
      -DSHELL_TASK_STACK_SIZE=2048\
      -DSHELL_USE_COMMON_TASK=0\
      -DUSE_RTOS=1"
)

mcux_add_iar_configuration(
    LD "--semihosting"
)

mcux_add_mdk_configuration(
    LD "--library_type=microlib"
)

include(${SdkRootDirPath}/examples/_boards/${board}/edgefast_bluetooth_examples/a2dp_source/${multicore_foldername}/reconfig.cmake OPTIONAL)

mcux_convert_binary(BINARY ${APPLICATION_BINARY_DIR}/${MCUX_SDK_PROJECT_NAME}.bin)