
cmake_minimum_required(VERSION 3.22.0)

include(${SdkRootDirPath}/cmake/extension/mcux.cmake)

project(call_gateway LANGUAGES C CXX ASM PROJECT_BOARD_PORT_PATH examples/_boards/${board}/edgefast_bluetooth_examples/call_gateway)

mcux_add_include(
 	BASE_PATH ${SdkRootDirPath}
    INCLUDES middleware/edgefast_bluetooth/boards/${board}/edgefast_bluetooth_examples/call_gateway
	TARGET edgefast_bluetooth_app.h
)
mcux_add_source(
	BASE_PATH ${SdkRootDirPath}
    SOURCES middleware/edgefast_bluetooth/boards/${board}/edgefast_bluetooth_examples/call_gateway/${multicore_foldername}/edgefast_bluetooth_app.h
    CONFIG True
    PREINCLUDE True
)

include(${SdkRootDirPath}/CMakeLists.txt)
mcux_add_source(
  BASE_PATH ${SdkRootDirPath}
  SOURCES
    middleware/edgefast_bluetooth/examples/call_gateway/call_gateway.c
    middleware/edgefast_bluetooth/examples/call_gateway/call_gateway.h
    middleware/edgefast_bluetooth/examples/call_gateway/ccp_server.c
    middleware/edgefast_bluetooth/examples/call_gateway/ccp_server.h
    middleware/edgefast_bluetooth/examples/call_gateway/main.c
    middleware/edgefast_bluetooth/examples/call_gateway/ringtone.h
    middleware/edgefast_bluetooth/examples/call_gateway/set_coordinator.c
    middleware/edgefast_bluetooth/examples/call_gateway/set_coordinator.h
    middleware/edgefast_bluetooth/examples/call_gateway/unicast_audio_client.c
    middleware/edgefast_bluetooth/examples/call_gateway/unicast_audio_client.h
)

mcux_add_include(
  BASE_PATH ${SdkRootDirPath}
  INCLUDES middleware/edgefast_bluetooth/examples/call_gateway
)

mcux_add_macro(
  CC "-DCFG_BLE\
      -DLPUART_RING_BUFFER_SIZE=1024U\
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

include(${SdkRootDirPath}/examples/_boards/${board}/edgefast_bluetooth_examples/call_gateway/${multicore_foldername}/reconfig.cmake OPTIONAL)

mcux_convert_binary(BINARY ${APPLICATION_BINARY_DIR}/${MCUX_SDK_PROJECT_NAME}.bin)