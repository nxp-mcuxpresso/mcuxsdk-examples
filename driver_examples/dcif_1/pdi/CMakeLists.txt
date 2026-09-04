cmake_minimum_required(VERSION 3.22.0)

include(${SdkRootDirPath}/cmake/extension/mcux.cmake)

project(dcif_pdi
    LANGUAGES C CXX ASM
    PROJECT_BOARD_PORT_PATH ${board_root}/${board}/driver_examples/dcif/pdi
)

include(${SdkRootDirPath}/CMakeLists.txt)

mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES examples/driver_examples/dcif_1/pdi/dcif_pdi.c
            ${board_root}/${board}/driver_examples/dcif/pdi/hardware_init.c
            ${board_root}/${board}/driver_examples/dcif/pdi/app.h
)

mcux_add_include(
    BASE_PATH ${SdkRootDirPath}
    INCLUDES ${board_root}/${board}/driver_examples/dcif/pdi
             ${board_root}/${board}
)

mcux_add_macro(
    CC "-DISI_CAMERA_QUEUE_SIZE=8"
)

include(${SdkRootDirPath}/${board_root}/${board}/driver_examples/dcif/pdi/reconfig.cmake OPTIONAL)

mcux_convert_binary(BINARY ${APPLICATION_BINARY_DIR}/${MCUX_SDK_PROJECT_NAME}.bin)
