include(${CMAKE_CURRENT_LIST_DIR}/${core_id}/reconfig.cmake OPTIONAL)
include(${SdkRootDirPath}/${board_root}/${board}/wifi_examples/common/reconfig.cmake OPTIONAL)
mcux_add_macro(
    CC "-DFSL_USDHC_ENABLE_SCATTER_GATHER_TRANSFER=1\
       -DSDMMCHOST_ENABLE_CACHE_LINE_ALIGN_TRANSFER=1\
       -DCONFIG_WIFI_SLIM_ROAM=1\
       -DCONFIG_WIFI_SLIM_STA=1\
       -DCONFIG_WIFI_SLIM_UAP=1\
       -DCONFIG_FREERTOS_LOW_MEMORY_FOOTPRINT=1\
       -DCONFIG_LWIP_LOW_MEM_FOOTPRINT=1\
       -DCONFIG_FW_DNLD_ASYNC=1"
)
