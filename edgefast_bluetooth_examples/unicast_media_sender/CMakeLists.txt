
cmake_minimum_required(VERSION 3.22.0)

include(${SdkRootDirPath}/cmake/extension/mcux.cmake)

project(unicast_media_sender LANGUAGES C CXX ASM PROJECT_BOARD_PORT_PATH examples/_boards/${board}/edgefast_bluetooth_examples/unicast_media_sender)

mcux_add_include(
 	BASE_PATH ${SdkRootDirPath}
    INCLUDES middleware/edgefast_bluetooth/boards/${board}/edgefast_bluetooth_examples/unicast_media_sender
	TARGET edgefast_bluetooth_app.h
)
mcux_add_source(
	BASE_PATH ${SdkRootDirPath}
    SOURCES middleware/edgefast_bluetooth/boards/${board}/edgefast_bluetooth_examples/unicast_media_sender/${multicore_foldername}/edgefast_bluetooth_app.h
    CONFIG True
    PREINCLUDE True
)

include(${SdkRootDirPath}/CMakeLists.txt)
mcux_add_source(
  BASE_PATH ${SdkRootDirPath}
  SOURCES
    middleware/edgefast_bluetooth/examples/unicast_media_sender/app_a2dp_sink.c
    middleware/edgefast_bluetooth/examples/unicast_media_sender/app_a2dp_sink.h
    middleware/edgefast_bluetooth/examples/unicast_media_sender/app_connect.c
    middleware/edgefast_bluetooth/examples/unicast_media_sender/app_connect.h
    middleware/edgefast_bluetooth/examples/unicast_media_sender/le_audio_common.c
    middleware/edgefast_bluetooth/examples/unicast_media_sender/le_audio_common.h
    middleware/edgefast_bluetooth/examples/unicast_media_sender/le_audio_service.c
    middleware/edgefast_bluetooth/examples/unicast_media_sender/le_audio_service.h
    middleware/edgefast_bluetooth/examples/unicast_media_sender/le_audio_shell.c
    middleware/edgefast_bluetooth/examples/unicast_media_sender/le_audio_shell.h
    middleware/edgefast_bluetooth/examples/unicast_media_sender/main.c
    middleware/edgefast_bluetooth/examples/unicast_media_sender/modules/lc3_codec.c
    middleware/edgefast_bluetooth/examples/unicast_media_sender/modules/lc3_codec.h
    middleware/edgefast_bluetooth/examples/unicast_media_sender/modules/wav_file.c
    middleware/edgefast_bluetooth/examples/unicast_media_sender/modules/wav_file.h
    middleware/edgefast_bluetooth/examples/unicast_media_sender/unicast_media_sender.c
    middleware/edgefast_bluetooth/examples/unicast_media_sender/unicast_media_sender.h
)

mcux_add_include(
  BASE_PATH ${SdkRootDirPath}
  INCLUDES
    middleware/edgefast_bluetooth/examples/unicast_media_sender
    middleware/edgefast_bluetooth/examples/unicast_media_sender/modules
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

include(${SdkRootDirPath}/examples/_boards/${board}/edgefast_bluetooth_examples/unicast_media_sender/${multicore_foldername}/reconfig.cmake OPTIONAL)

mcux_convert_binary(BINARY ${APPLICATION_BINARY_DIR}/${MCUX_SDK_PROJECT_NAME}.bin)