cmake_minimum_required(VERSION 3.22.0)

include(${SdkRootDirPath}/cmake/extension/mcux.cmake)

project(a-format_PWM_trigger LANGUAGES C CXX ASM PROJECT_BOARD_PORT_PATH ${board_root}/${board}/digital_encoder_examples/a-format/PWM_trigger)

include(${SdkRootDirPath}/CMakeLists.txt)
mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES examples/digital_encoder_examples/a-format/PWM_trigger/a-format_PWM_trigger.c
            examples/digital_encoder_examples/a-format/PWM_trigger/a-format.c
            examples/digital_encoder_examples/a-format/PWM_trigger/a-format.h
)

mcux_add_include(
    BASE_PATH ${SdkRootDirPath}
    INCLUDES ${board_root}/${board}/digital_encoder_examples/a-format/PWM_trigger
             ${board_root}/${board}/digital_encoder_examples/a-format/PWM_trigger/${core_id}
)

mcux_add_macro(
    CC "-DPRINTF_ADVANCED_ENABLE=1\
       -DPRINTF_FLOAT_ENABLE=1"
)

include(${SdkRootDirPath}/${board_root}/${board}/digital_encoder_examples/a-format/PWM_trigger/reconfig.cmake OPTIONAL)

mcux_convert_binary(BINARY ${APPLICATION_BINARY_DIR}/${MCUX_SDK_PROJECT_NAME}.bin)
