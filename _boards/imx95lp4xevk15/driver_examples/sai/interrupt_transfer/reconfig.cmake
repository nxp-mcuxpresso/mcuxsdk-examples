
include(${CMAKE_CURRENT_LIST_DIR}/${core_id}/reconfig.cmake)

mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES examples/driver_examples/sai/music.h
)

mcux_add_include(
    BASE_PATH ${SdkRootDirPath}
    INCLUDES examples/driver_examples/sai
)
