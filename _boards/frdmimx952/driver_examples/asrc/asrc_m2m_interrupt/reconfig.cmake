include(${CMAKE_CURRENT_LIST_DIR}/${core_id}/reconfig.cmake OPTIONAL)

mcux_remove_armgcc_linker_script(
        TARGETS debug release
        BASE_PATH ${SdkRootDirPath}
        LINKER devices/i.MX/i.MX952/MIMX9529/gcc/MIMX9529_cm7_ram.ld
)

mcux_add_armgcc_linker_script(
        TARGETS debug release
        BASE_PATH ${SdkRootDirPath}
        LINKER ${board_root}/frdmimx952/driver_examples/asrc/asrc_m2m_interrupt/cm7/MIMX95294_cm7_ram.ld
)

mcux_remove_iar_linker_script(
        TARGETS debug release
        BASE_PATH ${SdkRootDirPath}
        LINKER devices/i.MX/i.MX952/MIMX9529/iar/MIMX9529_cm7_ram.icf
)

mcux_add_iar_linker_script(
        TARGETS debug release
        BASE_PATH ${SdkRootDirPath}
        LINKER ${board_root}/frdmimx952/driver_examples/asrc/asrc_m2m_interrupt/cm7/MIMX95294_cm7_ram.icf
)
