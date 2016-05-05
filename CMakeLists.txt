cmake_minimum_required(VERSION 2.8.12)

project(Destroyer2D)

set(BUILD_SHARED_LIBS false)
set(GLFW_BUILD_EXAMPLES false)
set(GLFW_BUILD_TESTS false)
set(GLFW_CLIENT_LIBRARY opengl)
add_subdirectory(third_party/glfw-3.1.2)

if(NOT CMAKE_BUILD_TYPE)
    SET(CMAKE_BUILD_TYPE Release)
endif(NOT CMAKE_BUILD_TYPE)

SET(CMAKE_CXX_FLAGS "-Wall")
SET(CMAKE_CXX_FLAGS_RELEASE "-O2 -DNDEBUG")
SET(CMAKE_CXX_FLAGS_DEBUG "-g -DCOMPILE_TEST")

aux_source_directory(src SRCS)

add_executable(Destroyer2D ${SRCS})

set_target_properties(
    Destroyer2D
    PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
    INCLUDE_DIRECTORIES ${CMAKE_BINARY_DIR}/third_party/glfw-3.1.2/include
    CXX_STANDARD 11
    CXX_STANDARD_REQUIRED true
)

get_directory_property(glfw_LIBRARIES DIRECTORY third_party/glfw-3.1.2 DEFINITION glfw_LIBRARIES)
get_directory_property(CMAKE_THREAD_LIBS_INIT DIRECTORY third_party/glfw-3.1.2 DEFINITION CMAKE_THREAD_LIBS_INIT)
get_directory_property(RT_LIBRARY DIRECTORY third_party/glfw-3.1.2 DEFINITION RT_LIBRARY)

target_link_libraries(Destroyer2D glfw)
target_link_libraries(Destroyer2D ${glfw_LIBRARIES})
target_link_libraries(Destroyer2D "${CMAKE_THREAD_LIBS_INIT}" "${RT_LIBRARY}")

