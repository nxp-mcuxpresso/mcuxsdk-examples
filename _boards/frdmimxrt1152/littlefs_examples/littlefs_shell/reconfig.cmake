
mcux_add_include(
    BASE_PATH ${SdkRootDirPath}
    INCLUDES ${board_root}/${board}/littlefs_examples/littlefs_shell
)

mcux_add_macro(
    CC "-DLFS_NO_INTRINSICS=1\
       -DSDK_DEBUGCONSOLE_UART"
)
mcux_add_macro(
    TOOLCHAINS armgcc iar
    TARGETS hyperram_release release
    CC "-DLFS_NO_ASSERT"
)
