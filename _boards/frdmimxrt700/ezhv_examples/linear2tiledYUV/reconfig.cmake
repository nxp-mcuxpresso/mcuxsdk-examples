include(${CMAKE_CURRENT_LIST_DIR}/${core_id}/reconfig.cmake OPTIONAL)

mcux_add_macro(
    CC "-DEZHV_LINEAR_TO_TILE_EN=1"
)