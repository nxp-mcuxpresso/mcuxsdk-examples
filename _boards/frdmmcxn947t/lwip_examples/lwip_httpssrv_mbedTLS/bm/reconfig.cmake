mcux_remove_linker_symbol(
    SYMBOLS "__stack_size__=1000"
)
mcux_add_linker_symbol(
    SYMBOLS "__stack_size__=6000"
)

include(${CMAKE_CURRENT_LIST_DIR}/${core_id}/reconfig.cmake OPTIONAL)
