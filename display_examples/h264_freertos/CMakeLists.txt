
cmake_minimum_required(VERSION 3.22.0)

include(${SdkRootDirPath}/cmake/extension/mcux.cmake)

project(h264_freertos LANGUAGES C CXX ASM PROJECT_BOARD_PORT_PATH examples/_boards/${board}/display_examples/h264_freertos)

include(${SdkRootDirPath}/CMakeLists.txt)
mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES examples/display_examples/h264_freertos/h264_freertos.c
            examples/display_examples/h264_freertos/FreeRTOS_POSIX_portable.h
            examples/display_examples/h264_freertos/display.c
            examples/display_examples/h264_freertos/display.h
            examples/display_examples/h264_freertos/h264_dec.h
            examples/display_examples/h264_freertos/h264_dec.cpp
            examples/display_examples/h264_freertos/sdcard.c
            examples/display_examples/h264_freertos/sdcard.h
            examples/display_examples/h264_freertos/FreeRTOSConfig.h
            examples/display_examples/h264_freertos/freertos_libraries/abstractions/posix/include/FreeRTOS_POSIX/errno.h
            examples/display_examples/h264_freertos/freertos_libraries/abstractions/posix/include/FreeRTOS_POSIX/fcntl.h
            examples/display_examples/h264_freertos/freertos_libraries/abstractions/posix/include/FreeRTOS_POSIX/mqueue.h
            examples/display_examples/h264_freertos/freertos_libraries/abstractions/posix/include/FreeRTOS_POSIX/pthread.h
            examples/display_examples/h264_freertos/freertos_libraries/abstractions/posix/include/FreeRTOS_POSIX/sched.h
            examples/display_examples/h264_freertos/freertos_libraries/abstractions/posix/include/FreeRTOS_POSIX/semaphore.h
            examples/display_examples/h264_freertos/freertos_libraries/abstractions/posix/include/FreeRTOS_POSIX/signal.h
            examples/display_examples/h264_freertos/freertos_libraries/abstractions/posix/include/FreeRTOS_POSIX/time.h
            examples/display_examples/h264_freertos/freertos_libraries/abstractions/posix/include/FreeRTOS_POSIX/unistd.h
            examples/display_examples/h264_freertos/freertos_libraries/abstractions/posix/include/FreeRTOS_POSIX/utils.h
            examples/display_examples/h264_freertos/freertos_libraries/abstractions/posix/include/FreeRTOS_POSIX/sys/types.h
            examples/display_examples/h264_freertos/freertos_libraries/freertos_plus/standard/freertos_plus_posix/include/FreeRTOS_POSIX.h
            examples/display_examples/h264_freertos/freertos_libraries/freertos_plus/standard/freertos_plus_posix/include/FreeRTOS_POSIX_internal.h
            examples/display_examples/h264_freertos/freertos_libraries/freertos_plus/standard/freertos_plus_posix/include/FreeRTOS_POSIX_portable_default.h
            examples/display_examples/h264_freertos/freertos_libraries/freertos_plus/standard/freertos_plus_posix/include/FreeRTOS_POSIX_types.h
            examples/display_examples/h264_freertos/freertos_libraries/freertos_plus/standard/freertos_plus_posix/source/FreeRTOS_POSIX_clock.c
            examples/display_examples/h264_freertos/freertos_libraries/freertos_plus/standard/freertos_plus_posix/source/FreeRTOS_POSIX_mqueue.c
            examples/display_examples/h264_freertos/freertos_libraries/freertos_plus/standard/freertos_plus_posix/source/FreeRTOS_POSIX_pthread.c
            examples/display_examples/h264_freertos/freertos_libraries/freertos_plus/standard/freertos_plus_posix/source/FreeRTOS_POSIX_pthread_barrier.c
            examples/display_examples/h264_freertos/freertos_libraries/freertos_plus/standard/freertos_plus_posix/source/FreeRTOS_POSIX_pthread_cond.c
            examples/display_examples/h264_freertos/freertos_libraries/freertos_plus/standard/freertos_plus_posix/source/FreeRTOS_POSIX_pthread_mutex.c
            examples/display_examples/h264_freertos/freertos_libraries/freertos_plus/standard/freertos_plus_posix/source/FreeRTOS_POSIX_sched.c
            examples/display_examples/h264_freertos/freertos_libraries/freertos_plus/standard/freertos_plus_posix/source/FreeRTOS_POSIX_semaphore.c
            examples/display_examples/h264_freertos/freertos_libraries/freertos_plus/standard/freertos_plus_posix/source/FreeRTOS_POSIX_timer.c
            examples/display_examples/h264_freertos/freertos_libraries/freertos_plus/standard/freertos_plus_posix/source/FreeRTOS_POSIX_unistd.c
            examples/display_examples/h264_freertos/freertos_libraries/freertos_plus/standard/freertos_plus_posix/source/FreeRTOS_POSIX_utils.c
            examples/display_examples/h264_freertos/freertos_libraries/c_sdk/standard/common/include/private/iot_doubly_linked_list.h
            examples/_boards/${board}/sdmmc_config.h
            examples/_boards/${board}/sdmmc_config.c
            examples/_boards/${board}/FreeRTOSConfigBoard.h
)

mcux_add_include(
    BASE_PATH ${SdkRootDirPath}
    INCLUDES examples/_boards/${board}
             examples/_boards/${board}/display_examples/h264_freertos
             examples/_boards/${board}/display_examples/h264_freertos/${multicore_foldername}
             examples/display_examples/h264_freertos/
             examples/display_examples/h264_freertos/freertos_libraries/abstractions/posix/include
             examples/display_examples/h264_freertos/freertos_libraries/abstractions/posix/include/FreeRTOS_POSIX/sys
             examples/display_examples/h264_freertos/freertos_libraries/freertos_plus/standard/freertos_plus_posix/include
             examples/display_examples/h264_freertos/freertos_libraries/c_sdk/standard/common/include/private
)


# Add additional configuration
mcux_add_iar_configuration(
    LD "--config_def=__stack_size__=0x1000 --config_def=__heap_size__=0x1800000"
    CX "--no_exceptions"
)

mcux_add_mdk_configuration(
    LD "--library_type=standardlib --predefine=\"-D__stack_size__=0x1000\" --predefine=\"-D__heap_size__=0x1800000\""
)

mcux_add_armgcc_configuration(
    LD "-Xlinker --defsym=__stack_size__=0x1000 -Xlinker --defsym=__heap_size__=0x1800000"
)

mcux_add_macro(
    TOOLCHAINS armgcc
    CC "-DSDK_USE_SYSCALL_STUB=0"
)

include(${SdkRootDirPath}/examples/_boards/${board}/display_examples/h264_freertos/reconfig.cmake OPTIONAL)

mcux_convert_binary(BINARY ${APPLICATION_BINARY_DIR}/${MCUX_SDK_PROJECT_NAME}.bin)
