ExternalMCUXProject_Add(
    APPLICATION el2go_import_blob_s
    SOURCE_DIR  ${APP_DIR}/../el2go_import_blob_s
)

add_dependencies(${DEFAULT_IMAGE} el2go_import_blob_s)