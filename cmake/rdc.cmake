include(eigen)

include_directories(${PROJECT_SOURCE_DIR})

add_library(rdc "${PROJECT_SOURCE_DIR}/rdc/rdc.h")
set_target_properties(rdc PROPERTIES LINKER_LANGUAGE CXX)
target_link_libraries(rdc Eigen3::Eigen)
