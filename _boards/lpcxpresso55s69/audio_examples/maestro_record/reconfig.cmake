include(${CMAKE_CURRENT_LIST_DIR}/${core_id}/reconfig.cmake OPTIONAL)

mcux_add_include(
    BASE_PATH ${CMAKE_CURRENT_LIST_DIR}
    INCLUDES .
)

mcux_add_source(
    BASE_PATH ${CMAKE_CURRENT_LIST_DIR}
    SOURCES streamer_pcm.h
            streamer_pcm.c
            app.h
            app_definitions.h
)

mcux_remove_macro(
        CC "-DVIT_PROC=1"
        TOOLCHAINS armgcc mcux
)