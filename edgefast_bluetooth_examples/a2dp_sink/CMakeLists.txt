
cmake_minimum_required(VERSION 3.22.0)

include(${SdkRootDirPath}/cmake/extension/mcux.cmake)

project(a2dp_sink LANGUAGES C CXX ASM PROJECT_BOARD_PORT_PATH ${board_root}/${board}/edgefast_bluetooth_examples/a2dp_sink)

mcux_add_include(
 	BASE_PATH ${SdkRootDirPath}
    INCLUDES examples/_boards/${board}/edgefast_bluetooth_examples/a2dp_sink
	TARGET edgefast_bluetooth_app.h
)
mcux_add_source(
	BASE_PATH ${SdkRootDirPath}
    SOURCES examples/_boards/${board}/edgefast_bluetooth_examples/a2dp_sink/${multicore_foldername}/edgefast_bluetooth_app.h
    CONFIG True
    PREINCLUDE True
    PREINCLUDE_TYPE c_include cpp_include
)

include(${SdkRootDirPath}/CMakeLists.txt)
mcux_add_source(
  BASE_PATH ${SdkRootDirPath}
  SOURCES
    examples/edgefast_bluetooth_examples/a2dp_sink/app_a2dp_sink.c
    examples/edgefast_bluetooth_examples/a2dp_sink/app_a2dp_sink.h
    examples/edgefast_bluetooth_examples/a2dp_sink/app_connect.c
    examples/edgefast_bluetooth_examples/a2dp_sink/app_connect.h
    examples/edgefast_bluetooth_examples/a2dp_sink/main.c
)

mcux_add_include(
  BASE_PATH ${SdkRootDirPath}
  INCLUDES examples/edgefast_bluetooth_examples/a2dp_sink
)

mcux_add_macro(
  CC "-DLPUART_RING_BUFFER_SIZE=1024U\
      -DPRINTF_ADVANCED_ENABLE=1\
      -DSDK_DEBUGCONSOLE_UART=1\
      -DSDK_OS_FREE_RTOS\
      -DUSE_RTOS=1"
)

mcux_add_iar_configuration(
    LD "--semihosting"
)

mcux_add_mdk_configuration(
    LD "--library_type=microlib"
)

include(${SdkRootDirPath}/${board_root}/${board}/edgefast_bluetooth_examples/a2dp_sink/${multicore_foldername}/reconfig.cmake OPTIONAL)

mcux_convert_binary(BINARY ${APPLICATION_BINARY_DIR}/${MCUX_SDK_PROJECT_NAME}.bin)