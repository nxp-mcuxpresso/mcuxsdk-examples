mcux_add_source(
    SOURCES xip/evkbmimxrt1170_flexspi_nor_config.c
            xip/evkbmimxrt1170_flexspi_nor_config.h
	    ../../xmcd.h
	    ../../xmcd.c
	    ../../dcd.h
	    ../../dcd.c
)

mcux_add_include(
    INCLUDES ./xip
)
include(${CMAKE_CURRENT_LIST_DIR}/${core_id}/reconfig.cmake)
