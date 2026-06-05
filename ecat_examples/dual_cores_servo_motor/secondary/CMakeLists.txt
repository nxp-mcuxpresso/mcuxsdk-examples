#
# Copyright 2024 NXP
#
# SPDX-License-Identifier: BSD-3-Clause

cmake_minimum_required(VERSION 3.22.0)

include(${SdkRootDirPath}/cmake/extension/mcux.cmake)

project(dual_cores_ecat_servo_motor_secondary_core LANGUAGES C CXX ASM PROJECT_BOARD_PORT_PATH examples/_boards/${board}/ecat_examples/dual_cores_servo_motor/remote)
include(${SdkRootDirPath}/CMakeLists.txt)

mcux_add_source(
	SOURCES mu_ipc_shm_client.c
	SOURCES mu_ipc_shm_client.h
	SOURCES ipc_shm_client.c
	SOURCES ipc_shm_client.h
	SOURCES ../ipc_shm.c
	SOURCES ../ipc_shm.h
	SOURCES ../ipc_shm_mem.c
	SOURCES ../ipc_shm_mem.h
	SOURCES ../mu_ipc_shm.h 
	SOURCES ../cc.h
)

mcux_add_source(
	BASE_PATH ${SdkRootDirPath}
	SOURCES ${board_root}/${board}/ecat_examples/dual_cores_servo_motor/remote/${core_id}/main.c
	SOURCES ${board_root}/${board}/ecat_examples/dual_cores_servo_motor/remote/${core_id}/m1_pmsm_appconfig.h
	SOURCES ${board_root}/${board}/ecat_examples/dual_cores_servo_motor/remote/${core_id}/m2_pmsm_appconfig.h
	SOURCES ${board_root}/${board}/ecat_examples/dual_cores_servo_motor/remote/${core_id}/clock_config.c
	SOURCES ${board_root}/${board}/ecat_examples/dual_cores_servo_motor/remote/${core_id}/clock_config.h
	SOURCES ${board_root}/${board}/ecat_examples/dual_cores_servo_motor/remote/${core_id}/peripherals.c
	SOURCES ${board_root}/${board}/ecat_examples/dual_cores_servo_motor/remote/${core_id}/peripherals.h
	SOURCES ${board_root}/${board}/ecat_examples/dual_cores_servo_motor/remote/${core_id}/freemaster_cfg.h
	SOURCES ${board_root}/${board}/ecat_examples/dual_cores_servo_motor/remote/${core_id}/mc_periph_init.c
	SOURCES ${board_root}/${board}/ecat_examples/dual_cores_servo_motor/remote/${core_id}/mc_periph_init.h
	SOURCES ${board_root}/${board}/ecat_examples/dual_cores_servo_motor/remote/${core_id}/app.h
)

mcux_add_include(
    INCLUDES .
    INCLUDES ../
)

mcux_add_include(
    BASE_PATH ${SdkRootDirPath}
    INCLUDES ${board_root}/${board}/ecat_examples/dual_cores_servo_motor/remote/${core_id}
)

include(${SdkRootDirPath}/${board_root}/${board}/ecat_examples/dual_cores_servo_motor/remote/${core_id}/reconfig.cmake OPTIONAL)

mcux_add_configuration(
    CC "-D__USE_SHMEM"
)

mcux_add_linker_symbol(
    SYMBOLS "__use_shmem__=1 \
             __multicore__=1 \
            "
)

mcux_add_iar_configuration(
    CX "--diag_suppress=Pa082,Pa050"
)

mcux_add_macro(
    CC "-DMC_EXAMPLE"
)

mcux_remove_armgcc_configuration(
    CC "-Werror"
    CX "-Werror"
)

mcux_remove_iar_configuration(
    CC "--warnings_are_errors"
    CX "--warnings_are_errors"
)

mcux_remove_mdk_configuration(
    CC "-Werror"
    CX "-Werror"
)

mcux_convert_binary(
	BINARY ${APPLICATION_BINARY_DIR}/${MCUX_SDK_PROJECT_NAME}.bin
)

mcux_convert_binary(
    TOOLCHAINS armgcc mdk iar
    BINARY ${APPLICATION_BINARY_DIR}/${CONFIG_TOOLCHAIN}/core1_image.bin
)