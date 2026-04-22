mcux_add_include(
    BASE_PATH ${SdkRootDirPath}
    INCLUDES components/cmsis_drivers/cmsis_mcx_enet
             components/cmsis_drivers/cmsis_enet_phy
             arch/arm/CMSIS/Driver/Include
)


mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES ${board_root}/${board}/cmsis_driver_examples/enet/txrx_transfer/hardware_init.c
            ${board_root}/${board}/cmsis_driver_examples/enet/txrx_transfer/app.h
            components/cmsis_drivers/cmsis_mcx_enet/fsl_enet_cmsis.c
            components/cmsis_drivers/cmsis_mcx_enet/fsl_enet_cmsis.h
            components/cmsis_drivers/cmsis_enet_phy/fsl_enet_phy_cmsis.c
            components/cmsis_drivers/cmsis_enet_phy/fsl_enet_phy_cmsis.h
)

mcux_add_linker_symbol(
    SYMBOLS "__heap_size__=0x3000\
             __stack_size__=0x3000\
             __ram_vector_table__=1"
)
