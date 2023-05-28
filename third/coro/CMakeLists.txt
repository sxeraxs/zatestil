apply_patch(${CMAKE_CURRENT_SOURCE_DIR}/patch.txt ${CMAKE_CURRENT_SOURCE_DIR}/coro)
set(LIBCORO_BUILD_TESTS OFF CACHE INTERNAL "")
set(LIBCORO_CODE_COVERAGE OFF CACHE INTERNAL "")
set(LIBCORO_BUILD_EXAMPLES OFF CACHE INTERNAL "")

add_subdirectory(coro)
add_library(coro ALIAS libcoro)
target_compile_options(libcoro PRIVATE -Wall -Wextra -Werror -Wno-unused-parameter -Wno-reorder)
