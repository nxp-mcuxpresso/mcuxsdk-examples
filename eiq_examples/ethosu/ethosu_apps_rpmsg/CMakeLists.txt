cmake_minimum_required(VERSION 3.22.0)

include(${SdkRootDirPath}/cmake/extension/mcux.cmake)

project(ethosu_apps_rpmsg LANGUAGES C CXX ASM PROJECT_BOARD_PORT_PATH ${board_root}/${board}/eiq_examples/ethosu/ethosu_apps_rpmsg)

mcux_add_include(
  INCLUDES .
  )

mcux_add_source(
  SOURCES ethosu_apps_rpmsg.cpp
          ethosu_core_interface.h
  )

mcux_add_armgcc_configuration(
    CX "-Wno-maybe-uninitialized\
        -Wno-deprecated-declarations"
)
mcux_add_armgcc_configuration(
    CC "-Wno-unused-variable"
)

mcux_add_linker_symbol(
    SYMBOLS "__use_shmem__=1 \
             __multicore__=1 \
            "
)

mcux_add_mdk_configuration(
    LD "--library_type=standardlib"
)

include(${SdkRootDirPath}/CMakeLists.txt)
include(${SdkRootDirPath}/${board_root}/${board}/eiq_examples/ethosu/ethosu_apps_rpmsg/reconfig.cmake OPTIONAL)

mcux_convert_binary(BINARY ${APPLICATION_BINARY_DIR}/${MCUX_SDK_PROJECT_NAME}.bin)
