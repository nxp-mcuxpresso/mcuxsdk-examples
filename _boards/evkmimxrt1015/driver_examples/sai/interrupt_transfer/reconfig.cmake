
mcux_add_macro(
    CC "-DDEBUG_CONSOLE_ASSERT_DISABLE=1"
)

mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES examples/driver_examples/sai/music.h
)

mcux_add_include(
    BASE_PATH ${SdkRootDirPath}
    INCLUDES examples/driver_examples/sai
)
