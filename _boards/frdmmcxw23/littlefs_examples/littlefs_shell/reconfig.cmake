
mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES ${board_root}/${board}/littlefs_examples/littlefs_shell/peripherals.c
            ${board_root}/${board}/littlefs_examples/littlefs_shell/peripherals.h
)

mcux_add_macro(
    "-DMFLASH_FILE_BASEADDR=0x70000"
)
