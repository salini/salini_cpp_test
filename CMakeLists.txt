cmake_minimum_required(VERSION 3.22)

project(salini_cpp_test CXX)


## load in pkg-config support
find_package(PkgConfig)
pkg_check_modules(PC_ZeroMQ QUIET zmq)

## use the hint from above to find where 'zmq.hpp' is located
find_path(ZeroMQ_INCLUDE_DIR
        NAMES zmq.hpp
        PATHS ${PC_ZeroMQ_INCLUDE_DIRS}
        )

## use the hint from above to find the location of libzmq
find_library(ZeroMQ_LIBRARY
        NAMES zmq
        PATHS ${PC_ZeroMQ_LIBRARY_DIRS}
        )

# include external libraries
include_directories(ext/argparse/include)
include_directories(ext/spdlog/include)
include_directories(ext/json)
include_directories(ext/catch/include)

###########
add_compile_definitions(TEST_DIR="${CMAKE_CURRENT_SOURCE_DIR}/test")

include_directories(src)

add_subdirectory(src)

add_subdirectory(test)
