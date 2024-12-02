ExternalMCUXProject_Add(
    APPLICATION el2go_blob_test_s
    SOURCE_DIR  ${APP_DIR}/../el2go_blob_test_s
)

add_dependencies(${DEFAULT_IMAGE} el2go_blob_test_s)