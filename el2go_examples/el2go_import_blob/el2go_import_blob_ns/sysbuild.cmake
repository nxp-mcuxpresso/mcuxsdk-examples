ExternalMCUXProject_Add(
    APPLICATION el2go_import_blob_s
    SOURCE_DIR  ${APP_DIR}/../el2go_import_blob_s
)

add_dependencies(${DEFAULT_IMAGE} el2go_import_blob_s)

# Let's build the tfm_s_crypto_clients application second
add_dependencies(el2go_import_blob_s tfm_s_crypto_clients)