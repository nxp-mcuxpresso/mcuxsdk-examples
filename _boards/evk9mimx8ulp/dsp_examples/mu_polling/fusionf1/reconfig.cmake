mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES examples/_boards/evkmimx8ulp/dsp_examples/mu_polling/fusionf1/./hardware_init.c
            examples/_boards/evkmimx8ulp/dsp_examples/mu_polling/fusionf1/./app.h
)

mcux_add_include(
    BASE_PATH ${SdkRootDirPath}
    INCLUDES examples/_boards/evkmimx8ulp/dsp_examples/mu_polling/fusionf1/.
             examples/_boards/evkmimx8ulp
)
