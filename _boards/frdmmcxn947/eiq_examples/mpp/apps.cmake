#populate lists all applications built for this board

if(${EXAMPLE} MATCHES "all")
    file(STRINGS ${S}boards/${BOARD}/examples.conf LIST_EXAMPLE)
else()
    list(APPEND LIST_EXAMPLE
          ${EXAMPLE}
          )
endif()

if(${BUILD_TEST} MATCHES "all")
    file(STRINGS ${S}boards/${BOARD}/tests.conf LIST_TEST)
elseif (NOT ${BUILD_TEST} STREQUAL "")
    list(APPEND LIST_TEST
          ${BUILD_TEST}
          )
endif()

