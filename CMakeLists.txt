cmake_minimum_required(VERSION 3.21)

include(cmake/HunterGate.cmake)
cmake_host_system_information(RESULT HUNTER_JOBS_NUMBER QUERY NUMBER_OF_LOGICAL_CORES)

option(HUNTER_ROOT ${CMAKE_CURRENT_BINARY_DIR})

HunterGate(URL "https://github.com/cpp-pm/hunter/archive/v0.24.2.tar.gz"
        SHA1 "17fd3b9f0b8c271eac9b71e30a6aa1b9fecab299"
        LOCAL)

project(zatestil)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)
set(ROOT_DIR "${CMAKE_CURRENT_LIST_DIR}")
include(cmake/patch.cmake)
include(cmake/module.cmake)

set(CMAKE_CXX_FLAGS_DEBUG "-g3 -Wall -Wextra -Werror -Wno-stringop-overflow -flto -fcoroutines")
set(CMAKE_CXX_FLAGS_RELEASE "-O3 -Wall -Wextra -Werror -Wno-stringop-overflow -flto -fcoroutines")
set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "O2 -g1 -Wall -Wextra -Werror -Wno-stringop-overflow -flto -fcoroutines")

message(STATUS "CMAKE_CXX_FLAGS_DEBUG = ${CMAKE_CXX_FLAGS_DEBUG}")
message(STATUS "CMAKE_CXX_FLAGS_RELEASE = ${CMAKE_CXX_FLAGS_RELEASE}")
message(STATUS "CMAKE_CXX_FLAGS_RELWITHDEBINFO = ${CMAKE_CXX_FLAGS_RELWITHDEBINFO}")

set(BOOST_COMPONENTS)
list(APPEND BOOST_COMPONENTS system program_options thread context coroutine)

execute_process(COMMAND bash -c "git submodule update --init --recursive"
        WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}"
        ERROR_QUIET OUTPUT_STRIP_TRAILING_WHITESPACE)

add_subdirectory(third)
find_package(Threads REQUIRED)
find_package(GTest CONFIG REQUIRED)
find_package(spdlog CONFIG REQUIRED)
find_package(OpenSSL REQUIRED)
find_package(CURL CONFIG REQUIRED)
find_package(libsodium CONFIG REQUIRED)
find_package(Boost REQUIRED CONFIG COMPONENTS ${BOOST_COMPONENTS})

include_directories(include
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${Boost_INCLUDE_DIRS}
        third/sfe/sfe/include
        third/yas/yas/include
        third/tgbot/tgbot-cpp/include
        third/coro/coro/inc
        third/coro/coro/vendor/c-ares/c-ares/include
        third/coro/coro/vendor/tartanllama/expected/include
        ${CMAKE_CURRENT_BINARY_DIR}/third/coro/coro/vendor/c-ares/c-ares)

add_subdirectory(source)
add_subdirectory(test)
add_subdirectory(application)
