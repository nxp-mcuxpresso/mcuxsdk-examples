
mcux_add_macro(
    CC "-DSRTM_NETC_SERVICE_USED=1 \
       -DSCMI_LMM_POWER_CHANGE_PROCESSED=1"
)

mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES
    ${board_root}/${board}/cm7/app_srtm.c
    ${board_root}/${board}/cm7/app_srtm.h
    ${board_root}/${board}/demo_apps/netc_share/cm7/rpmsg_config.h
    ${board_root}/${board}/demo_apps/netc_share/cm7/lwip_netc_port.h
    ${board_root}/${board}/rsc_table.c
    ${board_root}/${board}/rsc_table.h
    ${board_root}/${board}/srtm_config.h
    middleware/multicore/remoteproc/remoteproc.h
)

mcux_add_include(
    BASE_PATH ${SdkRootDirPath}
    INCLUDES
    ${board_root}/${board}/demo_apps/netc_share/cm7
    ${board_root}/${board}/
    ${board_root}/${board}/cm7
    middleware/multicore/remoteproc
)

# Remove Linker File Configurations
mcux_remove_iar_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS debug release
    LINKER devices/i.MX/i.MX952/MIMX9529/iar/MIMX9529_cm7_ram.icf
)
mcux_remove_armgcc_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS debug release
    LINKER devices/i.MX/i.MX952/MIMX9529/gcc/MIMX9529_cm7_ram.ld
)

# Add Linker File Configurations
mcux_add_iar_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS debug release
    LINKER ${board_root}/frdmimx952/iar/MIMX95294_cm7_lwip_ram.icf
)
mcux_add_armgcc_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS debug release
    LINKER ${board_root}/frdmimx952/gcc/MIMX95294_cm7_lwip_ram.ld
)
