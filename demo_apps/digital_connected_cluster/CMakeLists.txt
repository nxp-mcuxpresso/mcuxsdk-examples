
cmake_minimum_required(VERSION 3.22.0)

include(${SdkRootDirPath}/cmake/extension/mcux.cmake)

project(edgefast_bluetooth_dcc_app LANGUAGES C CXX ASM PROJECT_BOARD_PORT_PATH ${board_root}/${board}/demo_apps/digital_connected_cluster)

mcux_add_include(
 	BASE_PATH ${SdkRootDirPath}
    INCLUDES examples/_boards/${board}/demo_apps/digital_connected_cluster
	TARGET_FILES edgefast_bluetooth_app.h
)
mcux_add_source(
	BASE_PATH ${SdkRootDirPath}
    SOURCES examples/_boards/${board}/demo_apps/digital_connected_cluster/${multicore_foldername}/edgefast_bluetooth_app.h
    CONFIG True
    PREINCLUDE True
    PREINCLUDE_TYPE c_include cpp_include
)

include(${SdkRootDirPath}/CMakeLists.txt)
mcux_add_source(
  BASE_PATH ${SdkRootDirPath}
  SOURCES
    examples/demo_apps/digital_connected_cluster/fsl_shell.h
    examples/demo_apps/digital_connected_cluster/fsl_shell.c
    examples/demo_apps/digital_connected_cluster/sco_audio_pl_ext.c
	examples/demo_apps/digital_connected_cluster/usb_host_config.h
	examples/demo_apps/digital_connected_cluster/wav_file.c
	examples/demo_apps/digital_connected_cluster/wav_file.h
	examples/demo_apps/digital_connected_cluster/a2dp_pl_media_48KHz.h
	examples/demo_apps/digital_connected_cluster/app_a2dp_sink.c
	examples/demo_apps/digital_connected_cluster/app_a2dp_sink.h
	examples/demo_apps/digital_connected_cluster/app_a2dp_source.c
	examples/demo_apps/digital_connected_cluster/app_a2dp_source.h
	examples/demo_apps/digital_connected_cluster/app_avrcp.c
	examples/demo_apps/digital_connected_cluster/app_avrcp.h
	examples/demo_apps/digital_connected_cluster/app_config.h
	examples/demo_apps/digital_connected_cluster/app_connect.c
	examples/demo_apps/digital_connected_cluster/app_connect.h
	examples/demo_apps/digital_connected_cluster/app_dcc.c
	examples/demo_apps/digital_connected_cluster/app_dcc.h
	examples/demo_apps/digital_connected_cluster/app_discover.c
	examples/demo_apps/digital_connected_cluster/app_discover.h
	examples/demo_apps/digital_connected_cluster/app_handsfree.c
	examples/demo_apps/digital_connected_cluster/app_handsfree.h
	examples/demo_apps/digital_connected_cluster/app_handsfree_ag.c
	examples/demo_apps/digital_connected_cluster/app_handsfree_ag.h
	examples/demo_apps/digital_connected_cluster/app_map_mce.c
	examples/demo_apps/digital_connected_cluster/app_map_mce.h
	examples/demo_apps/digital_connected_cluster/app_pbap_pce.c
	examples/demo_apps/digital_connected_cluster/app_pbap_pce.h
	examples/demo_apps/digital_connected_cluster/app_shell.c
	examples/demo_apps/digital_connected_cluster/app_shell.h
	examples/demo_apps/digital_connected_cluster/display_interface.c
	examples/demo_apps/digital_connected_cluster/display_interface.h
	examples/demo_apps/digital_connected_cluster/ringtone.h
	examples/demo_apps/digital_connected_cluster/main.c
)

mcux_add_include(
  BASE_PATH ${SdkRootDirPath}
  INCLUDES
    examples/demo_apps/digital_connected_cluster
#    middleware/edgefast_bluetooth/source/shell
)

mcux_add_macro(
  CC "-DLPUART_RING_BUFFER_SIZE=1024U\
      -DPRINTF_ADVANCED_ENABLE=1\
      -DSDK_DEBUGCONSOLE_UART=1\
      -DSDK_OS_FREE_RTOS\
      -DSHELL_TASK_STACK_SIZE=4000\
      -DUSE_RTOS=1\
      -DCONFIG_BT_BREDR=1\
      -DCONFIG_A2DP_CODEC_EXTERNAL=0\
      -DCONFIG_BT_A2DP_CODEC_CONTROL=1\
      -DCONFIG_BT_A2DP_WRITE_EXT=1\
      -DAVRCP_TG_CT=1\
      -DCONFIG_DEBUG_AVRCP=0\
      -DHFP_BRIDGING=1"
)

mcux_add_iar_configuration(
    LD "--semihosting"
)

mcux_add_mdk_configuration(
    LD "--library_type=microlib"
)

include(${SdkRootDirPath}/${board_root}/${board}/demo_apps/digital_connected_cluster/${multicore_foldername}/reconfig.cmake OPTIONAL)

mcux_convert_binary(BINARY ${APPLICATION_BINARY_DIR}/${MCUX_SDK_PROJECT_NAME}.bin)
