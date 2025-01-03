cmake_minimum_required(VERSION 3.22.0)

include(${SdkRootDirPath}/cmake/extension/mcux.cmake)

project(usb_pd_source_charger_bm_om13790host LANGUAGES C CXX ASM PROJECT_BOARD_PORT_PATH ${board_root}/${board}/usb_examples/usb_pd_source_charger_om13790host/bm)

include(${SdkRootDirPath}/CMakeLists.txt)
mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES examples/usb_examples/usb_pd_source_charger_om13790host/bm/pd_app.c
)

mcux_add_include(
    BASE_PATH ${SdkRootDirPath}
    INCLUDES examples/usb_examples/usb_pd_source_charger_om13790host/bm
)

include(${SdkRootDirPath}/${board_root}/${board}/usb_examples/usb_pd_source_charger_om13790host/bm/reconfig.cmake OPTIONAL)

mcux_convert_binary(BINARY ${APPLICATION_BINARY_DIR}/${MCUX_SDK_PROJECT_NAME}.bin)