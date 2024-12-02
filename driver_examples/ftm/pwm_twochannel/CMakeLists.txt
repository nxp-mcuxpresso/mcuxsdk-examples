cmake_minimum_required(VERSION 3.22.0)

include(${SdkRootDirPath}/cmake/extension/mcux.cmake)

project(ftm_pwm_twochannel LANGUAGES C CXX ASM PROJECT_BOARD_PORT_PATH examples/_boards/${board}/driver_examples/ftm/pwm_twochannel)

include(${SdkRootDirPath}/CMakeLists.txt)
mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES examples/driver_examples/ftm/pwm_twochannel/ftm_pwm_twochannel.c
)

mcux_add_include(
    BASE_PATH ${SdkRootDirPath}
    INCLUDES examples/_boards/${board}/driver_examples/ftm/pwm_twochannel
             examples/_boards/${board}/driver_examples/ftm/pwm_twochannel/${core_id}
)

include(${SdkRootDirPath}/examples/_boards/${board}/driver_examples/ftm/pwm_twochannel/reconfig.cmake OPTIONAL)

mcux_convert_binary(BINARY ${APPLICATION_BINARY_DIR}/${MCUX_SDK_PROJECT_NAME}.bin)