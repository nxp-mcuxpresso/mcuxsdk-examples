
mcux_add_macro(
    CC "-DSDK_I2C_BASED_COMPONENT_USED=1 \
       -DBOARD_USE_PCAL6524=1 \
       -DBOARD_USE_PCAL6408=1 \
       -DNETC_PORT_USE_INCLUDES=1 \
       -DUSE_RTOS=1 \
       -DPRINTF_ADVANCED_ENABLE=1"
)

mcux_add_linker_symbol(
    SYMBOLS "__stack_size__=2048\
             __heap_size__=25600"
)

mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES
    examples/_boards/${board}/FreeRTOSConfigBoard.h
    examples/_boards/${board}/demo_apps/netc_share/cm7/app_srtm.c
    examples/_boards/${board}/demo_apps/netc_share/cm7/app_srtm.h
    examples/_boards/${board}/demo_apps/netc_share/cm7/rpmsg_config.h
    examples/_boards/${board}/demo_apps/netc_share/cm7/lwip_netc_port.h
    examples/_boards/${board}/rsc_table.c
    examples/_boards/${board}/rsc_table.h
    examples/_boards/${board}/srtm_config.h
    middleware/multicore/remoteproc/remoteproc.h
)

mcux_add_include(
    BASE_PATH ${SdkRootDirPath}
    INCLUDES
    examples/_boards/${board}/demo_apps/netc_share/cm7
    examples/_boards/${board}/
    middleware/multicore/remoteproc
)

# Remove Linker File Configurations
mcux_remove_iar_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS debug release
    LINKER devices/i.MX/i.MX95/MIMX9596/iar/MIMX9596xxxxN_cm7_ram.icf
)
mcux_remove_armgcc_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS debug release
    LINKER devices/i.MX/i.MX95/MIMX9596/gcc/MIMX9596xxxxN_cm7_ram.ld
)

# Add Linker File Configurations
mcux_add_iar_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS debug release
    LINKER examples/_boards/imx95lpd5evk19/iar/MIMX9596_cm7_lwip_ram.icf
)
mcux_add_armgcc_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS debug release
    LINKER examples/_boards/imx95lpd5evk19/gcc/MIMX9596_cm7_lwip_ram.ld
)
