
mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES ${board_root}/kw45b41zevk/littlefs_examples/littlefs_shell/hardware_init.c
            ${board_root}/kw45b41zevk/littlefs_examples/littlefs_shell/app.h
            ${board_root}/${board}/littlefs_examples/littlefs_shell/peripherals.c
            ${board_root}/${board}/littlefs_examples/littlefs_shell/peripherals.h
)

mcux_add_include(
    BASE_PATH ${SdkRootDirPath}
    INCLUDES ${board_root}/kw45b41zevk/littlefs_examples/littlefs_shell
)

mcux_add_macro(
    TOOLCHAINS armgcc iar
    TARGETS hyperram_release release sdram_release
    CC "-DLFS_NO_ASSERT"
)
mcux_add_macro(
    CC "-DLFS_NO_INTRINSICS=1\
       -DSDK_DEBUGCONSOLE_UART"
)

mcux_add_armgcc_linker_script(
	TARGETS sdram_debug sdram_release debug release
    BASE_PATH ${SdkRootDirPath}
    LINKER ${board_root}/${board}/littlefs_examples/littlefs_shell/linker/K32W1480xxxA_ram.ld
)
