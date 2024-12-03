
mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
	SOURCES examples/_boards/${board}/littlefs_examples/littlefs_shell/peripherals.c
            examples/_boards/${board}/littlefs_examples/littlefs_shell/peripherals.h
)

mcux_add_include(
    BASE_PATH ${SdkRootDirPath}
    INCLUDES examples/_boards/${board}/littlefs_examples/littlefs_shell
)

mcux_add_macro(
    CC "-DLFS_NO_INTRINSICS=1\
       -DSDK_DEBUGCONSOLE_UART"
)
mcux_add_macro(
    TOOLCHAINS armgcc iar
    TARGETS hyperram_release release sdram_release
    CC "-DLFS_NO_ASSERT"
)
