cmake_minimum_required(VERSION 3.22.0)

include(${SdkRootDirPath}/cmake/extension/mcux.cmake)

project(t-format_sync_transfer LANGUAGES C CXX ASM PROJECT_BOARD_PORT_PATH ${board_root}/${board}/digital_encoder_examples/t-format/sync_transfer)

include(${SdkRootDirPath}/CMakeLists.txt)
mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES examples/digital_encoder_examples/t-format/sync_transfer/t-format_sync_transfer.c
            examples/digital_encoder_examples/t-format/sync_transfer/t-format.c
            examples/digital_encoder_examples/t-format/sync_transfer/t-format.h
)

mcux_add_include(
    BASE_PATH ${SdkRootDirPath}
    INCLUDES ${board_root}/${board}/digital_encoder_examples/t-format/sync_transfer
             ${board_root}/${board}/digital_encoder_examples/t-format/sync_transfer/${core_id}
)

mcux_add_macro(
    CC "-DPRINTF_ADVANCED_ENABLE=1\
       -DPRINTF_FLOAT_ENABLE=1"
)

# Remove default release target optimization
mcux_remove_armgcc_configuration(
    TARGETS release
    CC "-Os"
)

# Add optimization
mcux_add_armgcc_configuration(
    TARGETS release
    CC "-O0"
)

include(${SdkRootDirPath}/${board_root}/${board}/digital_encoder_examples/t-format/sync_transfer/reconfig.cmake OPTIONAL)

mcux_convert_binary(BINARY ${APPLICATION_BINARY_DIR}/${MCUX_SDK_PROJECT_NAME}.bin)
