
mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES ${board_root}/evkmimx8mn/driver_examples/asrc/asrc_m2m_sdma/pin_mux.c
            ${board_root}/evkmimx8mn/driver_examples/asrc/asrc_m2m_sdma/pin_mux.h
            ${board_root}/evkmimx8mn/driver_examples/asrc/asrc_m2m_sdma/./app.h
            ${board_root}/evkmimx8mn/driver_examples/asrc/asrc_m2m_sdma/./hardware_init.c
            middleware/multicore/remoteproc/empty_rsc_table.c
)

mcux_add_include(
    BASE_PATH ${SdkRootDirPath}
    INCLUDES ${board_root}/evkmimx8mn/driver_examples/asrc/asrc_m2m_sdma
             examples/driver_examples/easrc/asrc_m2m_sdma
             ${board_root}/evkmimx8mn
)
include(${SdkRootDirPath}/examples/_boards/${board}/project_common.cmake OPTIONAL)
