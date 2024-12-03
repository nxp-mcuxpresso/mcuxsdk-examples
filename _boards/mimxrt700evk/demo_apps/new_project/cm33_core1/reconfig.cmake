
mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES examples/_boards/${board}/project_template/${core_id}/peripherals.c
            examples/_boards/${board}/project_template/${core_id}/peripherals.h
            examples/_boards/${board}/project_template/${core_id}/pin_mux.c
            examples/_boards/${board}/project_template/${core_id}/pin_mux.h
)

mcux_add_include(
    BASE_PATH ${SdkRootDirPath}
    INCLUDES examples/_boards/${board}/demo_apps/new_project
             examples/_boards/${board}/project_template
             examples/_boards/${board}/project_template/${core_id}
)
