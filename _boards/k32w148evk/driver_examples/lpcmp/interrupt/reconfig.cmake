
mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES examples/_boards/kw45b41zevk/driver_examples/lpcmp/interrupt/./hardware_init.c
            examples/_boards/kw45b41zevk/driver_examples/lpcmp/interrupt/./app.h
)

mcux_add_include(
    BASE_PATH ${SdkRootDirPath}
    INCLUDES examples/_boards/kw45b41zevk/driver_examples/lpcmp/interrupt/.
)
