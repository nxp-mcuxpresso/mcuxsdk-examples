
mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES ${board_root}/evkmimx8mn/driver_examples/asrc/asrc_m2m_polling/pin_mux.c
            ${board_root}/evkmimx8mn/driver_examples/asrc/asrc_m2m_polling/pin_mux.h
            ${board_root}/evkmimx8mn/driver_examples/asrc/asrc_m2m_polling/./app.h
            ${board_root}/evkmimx8mn/driver_examples/asrc/asrc_m2m_polling/./hardware_init.c
            middleware/multicore/remoteproc/empty_rsc_table.c
)

mcux_add_include(
    BASE_PATH ${SdkRootDirPath}
    INCLUDES ${board_root}/evkmimx8mn/driver_examples/asrc/asrc_m2m_polling
             examples/driver_examples/easrc/asrc_m2m_polling
             ${board_root}/evkmimx8mn
)
include(${SdkRootDirPath}/examples/_boards/${board}/project_common.cmake OPTIONAL)
