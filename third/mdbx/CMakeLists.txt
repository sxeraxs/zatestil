set(MDBX_INSTALL_STATIC ${CMAKE_ARCHIVE_OUTPUT_DIRECTORY})
add_subdirectory(mdbx)
add_library(mdbx ALIAS mdbx-static)
