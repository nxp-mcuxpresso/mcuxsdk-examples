mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES examples/_boards/${board}/littlefs_examples/littlefs_shell/peripherals.c
            examples/_boards/${board}/littlefs_examples/littlefs_shell/peripherals.h
)

mcux_add_macro(
    CC "-DLFS_NO_INTRINSICS=1\
       -DSDK_DEBUGCONSOLE_UART\
	   -DSKIP_SYSCLK_INIT\
       -DXIP_BOOT_HEADER_DCD_ENABLE=1"
)

mcux_add_macro(
    TOOLCHAINS armgcc iar
    TARGETS hyperram_release release sdram_release
    CC "-DLFS_NO_ASSERT"
)

mcux_add_mdk_configuration(
    LD "--diag_suppress=L6329W"
)

mcux_remove_armgcc_linker_script(
		TARGETS debug release sdram_debug sdram_release
        BASE_PATH ${SdkRootDirPath}
        LINKER devices/RT/RT1020/MIMXRT1021/gcc/MIMXRT1021xxxxx_flexspi_nor.ld
)

mcux_remove_armgcc_linker_script(
		TARGETS debug release sdram_debug sdram_release
        BASE_PATH ${SdkRootDirPath}
        LINKER devices/RT/RT1020/MIMXRT1021/gcc/MIMXRT1021xxxxx_ram.ld
)

mcux_add_armgcc_linker_script(
    TARGETS debug release sdram_debug sdram_release
    BASE_PATH ${SdkRootDirPath}
    LINKER examples/_boards/${board}/littlefs_examples/littlefs_shell/linker/MIMXRT1021xxxxx_sdram.ld
)