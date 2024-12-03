# Override heap size
mcux_remove_linker_symbol(
    SYMBOLS "__heap_size__=40000"
)
mcux_add_linker_symbol(
    SYMBOLS "__heap_size__=50000"
)

include(${CMAKE_CURRENT_LIST_DIR}/${core_id}/reconfig.cmake OPTIONAL)