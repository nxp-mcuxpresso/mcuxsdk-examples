# Add common include paths
mcux_add_include(
    BASE_PATH ${SdkRootDirPath}
    INCLUDES
        ${project_board_port_path}
)

# Add example include path
mcux_add_include(
    INCLUDES
        example
)

# Add example tsa_data include paths
mcux_add_include(
    INCLUDES
        tsa_data
)

# Add common source files
mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES
        ${project_board_port_path}/main.c
)

# Add config file
mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES
        ${project_board_port_path}/freemaster_cfg.h
)

# Add example and TSA data header files
mcux_add_source(
    SOURCES
        example/freemaster_example.h
        tsa_data/data_example_pmp.h
        tsa_data/data_html_index.h
        tsa_data/data_img_file.h
        tsa_data/data_img_fmstrlogo.h
        tsa_data/data_img_logo.h
        tsa_data/example_data.h
)

# Use MicroLib library (IDE default option) for MDK linker
mcux_add_mdk_configuration(
    LD "--library_type=microlib"
)