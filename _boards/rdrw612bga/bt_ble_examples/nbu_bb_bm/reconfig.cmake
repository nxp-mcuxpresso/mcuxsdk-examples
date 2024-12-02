mcux_add_source(
    BASE_PATH ${SdkRootDirPath}/examples/_boards/${board}/bt_ble_examples/nbu_bb_bm
    SOURCES hardware_init.c
            pin_mux.c
            pin_mux.h
)

mcux_add_source(
    BASE_PATH ${SdkRootDirPath}/examples/bt_ble_examples/nbu_bb_bm
    SOURCES nbu_bb_bm.c
)

mcux_add_source(
    BASE_PATH ${SdkRootDirPath}/examples/_boards/${board}
    PREINCLUDE TRUE
    SOURCES monolithic_config.h
)

mcux_add_include(
    BASE_PATH ${SdkRootDirPath}
    INCLUDES examples/_boards/${board}/bt_ble_examples/nbu_bb_bm
)




# Add additional configuration
mcux_add_iar_configuration(
    LD "--semihosting"
    CX "--diag_suppress Pa167,Pe177,Pe191,Pe546"
)
mcux_remove_macro(
    TOOLCHAINS iar
    CC "-DSDK_OS_FREE_RTOS -DMCUXPRESSO_SDK -DPRINTF_FLOAT_ENABLE -DPRINTF_ADVANCED_ENABLE"
)
mcux_add_macro(
    CC "-DCPU3\
       -DOSA_USED\
       -DBOOT_HEADER_ENABLE=1\
       -DFSL_OSA_TASK_ENABLE\
       -DFSL_OSA_BM_TIMER_CONFIG\
       -DFSL_OSA_MAIN_FUNC_ENABLE=0\
       -DSERIAL_PORT_TYPE_UART=1\
       -DgMemManagerLightExtendHeapAreaUsage=1"
)

mcux_add_mdk_configuration(
    LD "--diag_suppress=6329\
       --diag_suppress=6319\
       --diag_suppress=6675\
       --diag_suppress=6775\
       --legacyalign\
       --diag_suppress=3912"
)

# Add or remove Linker File Configurations
mcux_remove_iar_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS flash_debug flash_release
    LINKER devices/Wireless/RW/RW612/iar/RW612_flash.icf
)
mcux_remove_armgcc_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS flash_debug flash_release
    LINKER devices/Wireless/RW/RW612/gcc/RW612_flash.ld
)

# Add or remove Linker File Configurations
mcux_add_iar_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS flash_debug flash_release
    LINKER examples/_boards/${board}/bt_ble_examples/nbu_bb_bm/linker/RW61x_flash.icf
)
mcux_add_armgcc_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS flash_debug flash_release
    LINKER examples/_boards/${board}/bt_ble_examples/nbu_bb_bm/linker/RW61x_flash.ld
)
