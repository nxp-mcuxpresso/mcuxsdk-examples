

mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES examples/_boards/${board}/littlefs_examples/littlefs_shell/peripherals.c
            examples/_boards/${board}/littlefs_examples/littlefs_shell/peripherals.h
)




# Add additional configuration
mcux_add_macro(
    CC "-DLFS_NO_INTRINSICS=1\
       -DSDK_DEBUGCONSOLE_UART"
)
mcux_add_macro(
    TOOLCHAINS iar
    TARGETS release
    CC "-DLFS_NO_ASSERT"
)
mcux_add_macro(
    TOOLCHAINS iar
    TARGETS sdram_release
    CC "-DLFS_NO_ASSERT"
)
mcux_add_macro(
    TOOLCHAINS iar
    TARGETS hyperram_release
    CC "-DLFS_NO_ASSERT"
)
mcux_add_macro(
    TOOLCHAINS armgcc
    TARGETS release
    CC "-DLFS_NO_ASSERT"
)
mcux_add_macro(
    TOOLCHAINS armgcc
    TARGETS sdram_release
    CC "-DLFS_NO_ASSERT"
)
mcux_add_macro(
    TOOLCHAINS armgcc
    TARGETS hyperram_release
    CC "-DLFS_NO_ASSERT"
)
