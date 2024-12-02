
mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES examples/demo_apps/coremark/coremark_bench/core_list_join.c
            examples/demo_apps/coremark/coremark_bench/core_matrix.c
            examples/demo_apps/coremark/coremark_bench/core_state.c
            examples/demo_apps/coremark/coremark_bench/core_util.c
            examples/demo_apps/coremark/coremark_bench/coremark_bench.h
            examples/demo_apps/coremark/coremark_config/core_portme.c
            examples/demo_apps/coremark/coremark_config/core_portme.h
            examples/demo_apps/coremark/coremark_config/core_type.h
            examples/_boards/${board}/FlashKLxx256KROM_with_config_write_enabled.board
)

mcux_add_include(
    BASE_PATH ${SdkRootDirPath}
    INCLUDES examples/demo_apps/coremark/coremark_config
)

mcux_add_macro(
    CC "-DITERATIONS=12000\
       -DPRINTF_FLOAT_ENABLE=1\
       -DPRINTF_ADVANCED_ENABLE=1\
       -DFRDM_MCXC444\
       -DFREEDOM"
)

mcux_remove_iar_configuration(
    CC "--misra2004"
)
