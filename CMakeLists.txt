cmake_minimum_required(VERSION 3.21)

include(cmake/HunterGate.cmake)
cmake_host_system_information(RESULT HUNTER_JOBS_NUMBER QUERY NUMBER_OF_LOGICAL_CORES)

HunterGate(
        URL "https://github.com/cpp-pm/hunter/archive/v0.24.2.tar.gz"
        SHA1 "17fd3b9f0b8c271eac9b71e30a6aa1b9fecab299"
        LOCAL
)

project(zatestil)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)
set(ROOT_DIR "${CMAKE_CURRENT_LIST_DIR}")
include(cmake/module.cmake)

set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -D_DEBUG")
set(NDEBUG_FLAGS "")

set(NDEBUG_FLAGS "-DNDEBUG -flto")

set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} ${NDEBUG_FLAGS}")
set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "${CMAKE_CXX_FLAGS_RELWITHDEBINFO} ${NDEBUG_FLAGS}")

set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g -O0")

set(BOOST_COMPONENTS)
list(APPEND BOOST_COMPONENTS system program_options thread context coroutine)

add_subdirectory(third)
find_package(Threads REQUIRED)
find_package(GTest CONFIG REQUIRED)
find_package(spdlog CONFIG REQUIRED)
find_package(websocketpp CONFIG REQUIRED)
find_package(Boost REQUIRED CONFIG COMPONENTS ${BOOST_COMPONENTS})

include_directories(include)
include_directories(${CMAKE_CURRENT_SOURCE_DIR})
include_directories(${Boost_INCLUDE_DIRS})
include_directories(third/sfe/sfe/include)
include_directories(third/yas/yas/include)
include_directories(third/tgbot/tgbot-cpp/include)

add_subdirectory(source)
add_subdirectory(test)
add_subdirectory(application)
