
cmake_minimum_required(VERSION 3.22.0)

include(${SdkRootDirPath}/cmake/extension/mcux.cmake)

project(umr2bms LANGUAGES C CXX ASM PROJECT_BOARD_PORT_PATH ${board_root}/${board}/edgefast_bluetooth_examples/umr2bms)

mcux_add_include(
 	BASE_PATH ${SdkRootDirPath}
    INCLUDES examples/_boards/${board}/edgefast_bluetooth_examples/umr2bms
	TARGET_FILES edgefast_bluetooth_app.h
)
mcux_add_source(
	BASE_PATH ${SdkRootDirPath}
    SOURCES examples/_boards/${board}/edgefast_bluetooth_examples/umr2bms/${multicore_foldername}/edgefast_bluetooth_app.h
    CONFIG True
    PREINCLUDE True
    PREINCLUDE_TYPE c_include cpp_include
)

include(${SdkRootDirPath}/CMakeLists.txt)
mcux_add_source(
  BASE_PATH ${SdkRootDirPath}
  SOURCES
    examples/edgefast_bluetooth_examples/umr2bms/broadcast_media_sender.c
    examples/edgefast_bluetooth_examples/umr2bms/broadcast_media_sender.h
    examples/edgefast_bluetooth_examples/umr2bms/le_audio_common.c
    examples/edgefast_bluetooth_examples/umr2bms/le_audio_common.h
    examples/edgefast_bluetooth_examples/umr2bms/le_audio_shell.c
    examples/edgefast_bluetooth_examples/umr2bms/le_audio_shell.h
    examples/edgefast_bluetooth_examples/umr2bms/main.c
    examples/edgefast_bluetooth_examples/umr2bms/umr2bms.c
    examples/edgefast_bluetooth_examples/umr2bms/umr2bms.h
    examples/edgefast_bluetooth_examples/umr2bms/unicast_media_receiver.c
    examples/edgefast_bluetooth_examples/umr2bms/unicast_media_receiver.h
    examples/edgefast_bluetooth_examples/config/mbedtls_user_config.h
)

mcux_add_include(
  BASE_PATH ${SdkRootDirPath}
  INCLUDES
    examples/edgefast_bluetooth_examples/umr2bms
    examples/edgefast_bluetooth_examples/umr2bms/modules
    middleware/edgefast_bluetooth/source/impl/ethermind/controller/configs/mbedtls
    examples/edgefast_bluetooth_examples/config
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
      -DMBEDTLS_USER_CONFIG_FILE=\\\"mbedtls_user_config.h\\\"\
      -DUSE_RTOS=1"
)

mcux_add_iar_configuration(
    LD "--semihosting"
)

mcux_add_mdk_configuration(
    LD "--library_type=microlib"
)

include(${SdkRootDirPath}/${board_root}/${board}/edgefast_bluetooth_examples/umr2bms/${multicore_foldername}/reconfig.cmake OPTIONAL)

mcux_convert_binary(BINARY ${APPLICATION_BINARY_DIR}/${MCUX_SDK_PROJECT_NAME}.bin)