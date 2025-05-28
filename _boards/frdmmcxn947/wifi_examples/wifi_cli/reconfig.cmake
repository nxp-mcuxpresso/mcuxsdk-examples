include(${CMAKE_CURRENT_LIST_DIR}/${core_id}/reconfig.cmake OPTIONAL)
include(${SdkRootDirPath}/${board_root}/${board}/wifi_examples/common/reconfig.cmake OPTIONAL)
mcux_add_macro(
    CC "-DCONFIG_FW_DNLD_ASYNC=1"
)
