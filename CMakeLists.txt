cmake_minimum_required(VERSION 3.18) # For SOURCE_SUBDIR in FetchContent_Declare 

project(rdc CXX)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
add_compile_options(-Wall -Wextra -Werror)

enable_testing()

option(RDC_BUILD_EXAMPLE "Builds the rdc example." ON)
option(RDC_BUILD_TESTS "Builds the rdc tests." ON)
option(RDC_BUILD_BENCHMARKS "Builds the rdc benchmarks." ON)

if(RDC_BUILD_EXAMPLE)
  include(example)
endif()

if(RDC_BUILD_TESTS)
  include(tests)
endif()

if(RDC_BUILD_BENCHMARKS)
  include(benchmarks)
endif()

include(rdc)