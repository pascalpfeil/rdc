include(FetchContent)
set(FETCHCONTENT_QUIET ON)
set(FETCHCONTENT_UPDATES_DISCONNECTED ON)
set(BUILD_SHARED_LIBS OFF)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)
set(BUILD_TESTING OFF)
find_package(Git REQUIRED)

message(STATUS "Fetching deps")
list(APPEND CMAKE_MESSAGE_INDENT "  ")


list(POP_BACK CMAKE_MESSAGE_INDENT)
message(STATUS "Done fetching deps.")