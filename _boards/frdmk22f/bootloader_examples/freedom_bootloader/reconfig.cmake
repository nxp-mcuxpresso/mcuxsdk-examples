
mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES middleware/mcu_bootloader/boards/${board}/bootloader/pin_mux.c
            middleware/mcu_bootloader/boards/${board}/bootloader/pin_mux.h
            middleware/mcu_bootloader/boards/${board}/bootloader/clock_config.c
            middleware/mcu_bootloader/boards/${board}/bootloader/clock_config.h
)

mcux_add_include(
    BASE_PATH ${SdkRootDirPath}
    INCLUDES middleware/mcu_bootloader/boards/${board}/bootloader
)

mcux_add_macro(
    TOOLCHAINS mdk
    AS "-DBL_HAS_BOOTLOADER_CONFIG=0"
)
mcux_add_macro(
    CC "-DFREEDOM\
       -DBL_TARGET_FLASH\
       -DFRDM_K22F\
       -DUSB_STACK_BM\
       -DFSL_OSA_BM_TIMER_CONFIG=FSL_OSA_BM_TIMER_NONE"
)
mcux_remove_iar_configuration(
    CC  "-DDEBUG"
    TARGETS release
)
mcux_remove_iar_configuration(
    CC  "-DNDEBUG"
    TARGETS debug
)
mcux_remove_mdk_configuration(
    CC  "-DDEBUG"
    TARGETS release
)
mcux_remove_mdk_configuration(
    CC  "-DNDEBUG"
    TARGETS debug
)
mcux_remove_armgcc_configuration(
    CC  "-DDEBUG"
    TARGETS release
)
mcux_remove_armgcc_configuration(
    CC  "-DNDEBUG"
    TARGETS debug
)