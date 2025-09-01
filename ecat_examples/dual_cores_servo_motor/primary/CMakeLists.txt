#
# Copyright 2025 NXP
#
# SPDX-License-Identifier: BSD-3-Clause

cmake_minimum_required(VERSION 3.22.0)

include(${SdkRootDirPath}/cmake/extension/mcux.cmake)

project(dual_cores_ecat_servo_motor_primary_core LANGUAGES C CXX ASM PROJECT_BOARD_PORT_PATH ${board_root}/${board}/ecat_examples/dual_cores_servo_motor/master/)

include(${SdkRootDirPath}/CMakeLists.txt)

include(${SdkRootDirPath}/${board_root}/${board}/ecat_examples/dual_cores_servo_motor/reconfig.cmake OPTIONAL)
include(${SdkRootDirPath}/${board_root}/${board}/ecat_examples/dual_cores_servo_motor/master/reconfig.cmake OPTIONAL)
include(${SdkRootDirPath}/${board_root}/${board}/ecat_examples/dual_cores_servo_motor/master/${core_id}/reconfig.cmake OPTIONAL)

mcux_add_source(
    SOURCES mu_ipc_shm_host.c
    SOURCES mu_ipc_shm_host.h
    SOURCES ipc_shm_host.c
    SOURCES ipc_shm_host.h
    SOURCES ecat_hw.h
    SOURCES ../ipc_shm.c
    SOURCES ../ipc_shm.h
    SOURCES ../ipc_shm_mem.c
    SOURCES ../ipc_shm_mem.h
    SOURCES ../mu_ipc_shm.h
    SOURCES ../cc.h
    SOURCES SSC/Src/cia402appl.c
    SOURCES SSC/Src/applInterface.h
    SOURCES SSC/Src/cia402appl.h
    SOURCES SSC/Src/coeappl.h
    SOURCES SSC/Src/ecatappl.h
    SOURCES SSC/Src/coeappl.h
    SOURCES SSC/Src/ecatappl.h
    SOURCES SSC/Src/ecatcoe.h
    SOURCES SSC/Src/ecatslv.c
    SOURCES SSC/Src/esc.h
    SOURCES SSC/Src/mailbox.h
    SOURCES SSC/Src/objdef.h
    SOURCES SSC/Src/sdoserv.h
    SOURCES SSC/Src/coeappl.c
    SOURCES SSC/Src/ecatappl.c
    SOURCES SSC/Src/ecatcoe.c
    SOURCES SSC/Src/ecat_def.h
    SOURCES SSC/Src/ecatslv.h
    SOURCES SSC/Src/mailbox.c
    SOURCES SSC/Src/objdef.c
    SOURCES SSC/Src/sdoserv.c
)

mcux_add_include(
    BASE_PATH ${SdkRootDirPath}
    INCLUDES ${board_root}/${board}/ecat_examples/dual_cores_servo_motor/master/${core_id}/
)

mcux_add_include(
    INCLUDES .
    INCLUDES ../
    INCLUDES ./SSC
    INCLUDES ./SSC/Src
)

mcux_add_configuration(
    CC "-D__USE_SHMEM"
)

mcux_add_linker_symbol(
    SYMBOLS "__use_shmem__=1 \
             __multicore__=1 \
            "
)

mcux_convert_binary(BINARY ${APPLICATION_BINARY_DIR}/${MCUX_SDK_PROJECT_NAME}.bin)
