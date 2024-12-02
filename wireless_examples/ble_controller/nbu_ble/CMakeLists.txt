cmake_minimum_required(VERSION 3.22.0)

include(${SdkRootDirPath}/cmake/extension/mcux.cmake)

project(nbu_ble LANGUAGES C CXX ASM 
    PROJECT_BOARD_PORT_PATH middleware/wireless/ble_controller/src/KW4x
)

include(${SdkRootDirPath}/CMakeLists.txt)

mcux_add_configuration(
    CC "        -DNDEBUG \
				-DgBleBondIdentityHeaderSize_c=56 \
				-DCPU_KW45B41Z83AFPA_NBU \
				-DPRINTF_FLOAT_ENABLE=0 \
				-DSCANF_FLOAT_ENABLE=0 \
				-DPRINTF_ADVANCED_ENABLE=0\
				-DSCANF_ADVANCED_ENABLE=0\
				-DFSL_OSA_TASK_ENABLE=1\
				-DSDK_COMPONENT_INTEGRATION=1\
				-DgUseHciTransportUpward_d=1\
				-DNBU_PROJECT_ENABLE=1\
				-DUSE_RAW_IMU_ADAPTER=0\
				-DHAL_RPMSG_SELECT_ROLE=1\
				-DSERIAL_PORT_TYPE_RPMSG=1\
				-DRPMSG_ADAPTER_NON_BLOCKING_MODE=1\
				-DSERIAL_MANAGER_NON_BLOCKING_MODE=1\
				-DHCI_FREE_RxBuffer=0\
				-DKW45_A0_SUPPORT=0\
				-DRF_OSC_26MHZ=0\
				-DTEST_BUILD_COEX=0\
				-DARM_MATH_CM3=1\
				-DgNbu_Hadm_d=1"
)

mcux_add_linker_symbol(
    SYMBOLS "__use_shmem__=1"
)

mcux_add_include(
    INCLUDES .
             ../
			 interface
)
	
# We would need a bluetooth component that is simply exposing some HCI defines
mcux_add_include(
    INCLUDES ../../../../../middleware/wireless/bluetooth/host/interface
	INCLUDES ../../../../../middleware/wireless/bluetooth/hci_transport/interface
	INCLUDES ../../../../../middleware/wireless/ble_controller/lib/threadx_inc
)

include(${SdkRootDirPath}/examples/_boards/${board}/wireless_examples/ble_controller/nbu_ble/${multicore_foldername}/reconfig.cmake OPTIONAL)

mcux_convert_binary(
    TOOLCHAINS armgcc mdk iar
    BINARY ${APPLICATION_BINARY_DIR}/${CONFIG_TOOLCHAIN}/core1_image.bin
)