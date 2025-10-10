
cmake_minimum_required(VERSION 3.22.0)

include(${SdkRootDirPath}/cmake/extension/mcux.cmake)

project(handsfree_ag LANGUAGES C CXX ASM PROJECT_BOARD_PORT_PATH ${board_root}/${board}/edgefast_bluetooth_examples/handsfree_ag)

mcux_add_include(
 	BASE_PATH ${SdkRootDirPath}
    INCLUDES examples/_boards/${board}/edgefast_bluetooth_examples/handsfree_ag
	TARGET_FILES edgefast_bluetooth_app.h
)
mcux_add_source(
	BASE_PATH ${SdkRootDirPath}
    SOURCES examples/_boards/${board}/edgefast_bluetooth_examples/handsfree_ag/${multicore_foldername}/edgefast_bluetooth_app.h
    CONFIG True
    PREINCLUDE True
    PREINCLUDE_TYPE c_include cpp_include
)

include(${SdkRootDirPath}/CMakeLists.txt)
mcux_add_source(
  BASE_PATH ${SdkRootDirPath}
  SOURCES
    examples/edgefast_bluetooth_examples/handsfree_ag/app_connect.c
    examples/edgefast_bluetooth_examples/handsfree_ag/app_connect.h
    examples/edgefast_bluetooth_examples/handsfree_ag/app_discover.c
    examples/edgefast_bluetooth_examples/handsfree_ag/app_discover.h
    examples/edgefast_bluetooth_examples/handsfree_ag/app_handsfree_ag.c
    examples/edgefast_bluetooth_examples/handsfree_ag/app_handsfree_ag.h
    examples/edgefast_bluetooth_examples/handsfree_ag/app_shell.c
    examples/edgefast_bluetooth_examples/handsfree_ag/app_shell.h
    examples/edgefast_bluetooth_examples/handsfree_ag/main.c
    examples/edgefast_bluetooth_examples/handsfree_ag/ringtone.h
    examples/edgefast_bluetooth_examples/handsfree_ag/sco_audio_pl_ext.c
    examples/edgefast_bluetooth_examples/config/mbedtls_user_config.h
)

mcux_add_include(
  BASE_PATH ${SdkRootDirPath}
  INCLUDES examples/edgefast_bluetooth_examples/handsfree_ag
           examples/edgefast_bluetooth_examples/config
)

mcux_add_macro(
  CC "-DLPUART_RING_BUFFER_SIZE=1024U\
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

include(${SdkRootDirPath}/${board_root}/${board}/edgefast_bluetooth_examples/handsfree_ag/${multicore_foldername}/reconfig.cmake OPTIONAL)

mcux_convert_binary(BINARY ${APPLICATION_BINARY_DIR}/${MCUX_SDK_PROJECT_NAME}.bin)