mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES ${board_root}/evkmimx8ulp/dsp_examples/sema42/fusionf1/./hardware_init.c
            ${board_root}/evkmimx8ulp/dsp_examples/sema42/fusionf1/./app.h
)

mcux_add_include(
    BASE_PATH ${SdkRootDirPath}
    INCLUDES ${board_root}/evkmimx8ulp/dsp_examples/sema42/fusionf1/.
             ${board_root}/evkmimx8ulp
)
