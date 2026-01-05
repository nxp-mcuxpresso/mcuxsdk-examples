
mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES ${board_root}/${board}/project_template/${core_id}/peripherals.c
            ${board_root}/${board}/project_template/${core_id}/peripherals.h
            ${board_root}/${board}/project_template/${core_id}/pin_mux.c
            ${board_root}/${board}/project_template/${core_id}/pin_mux.h
)

mcux_add_include(
    BASE_PATH ${SdkRootDirPath}
    INCLUDES ${board_root}/${board}/demo_apps/new_project
             ${board_root}/${board}/project_template
             ${board_root}/${board}/project_template/${core_id}
)
