cmake_minimum_required(VERSION 2.8.12)

project(Destroyer2D)

set(BUILD_SHARED_LIBS OFF CACHE BOOL "Build shared libraries" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "Build the GLFW example programs" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "Build the GLFW test programs" FORCE)
set(GLFW_BUILD_DOCS ON CACHE BOOL "Build the GLFW documentation" FORCE)
set(GLFW_INSTALL OFF "Generate installation target" FORCE)
add_subdirectory(third_party/glfw-3.1.2)

set(BOX2D_INSTALL OFF CACHE BOOL "Install Box2D libs, includes, and CMake scripts" FORCE)
set(BOX2D_INSTALL_DOC OFF CACHE BOOL "Install Box2D documentation" FORCE)
set(BOX2D_BUILD_SHARED OFF CACHE BOOL "Build Box2D shared libraries" FORCE)
set(BOX2D_BUILD_STATIC ON CACHE BOOL "Build Box2D static libraries" FORCE)
set(BOX2D_BUILD_EXAMPLES OFF CACHE BOOL "Build Box2D examples" FORCE)
set(BOX2D_BUILD_UNITTESTS OFF CACHE BOOL "Build Box2D Unit Tests" FORCE)
add_subdirectory(third_party/liquidfun/Box2D)

if(NOT CMAKE_BUILD_TYPE)
    SET(CMAKE_BUILD_TYPE Release)
endif(NOT CMAKE_BUILD_TYPE)

SET(CMAKE_CXX_FLAGS "-Wall")
SET(CMAKE_CXX_FLAGS_RELEASE "-O2 -DNDEBUG")
SET(CMAKE_CXX_FLAGS_DEBUG "-g -DCOMPILE_TEST")

aux_source_directory(src SRCS)

add_executable(Destroyer2D ${SRCS})

target_include_directories(
    Destroyer2D
    PRIVATE ${CMAKE_BINARY_DIR}/third_party/glfw-3.1.2/include
    PRIVATE ${CMAKE_BINARY_DIR}/third_party/liquidfun/Box2D
)

set_target_properties(
    Destroyer2D
    PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
    CXX_STANDARD 11
    CXX_STANDARD_REQUIRED true
)

get_directory_property(glfw_LIBRARIES DIRECTORY third_party/glfw-3.1.2 DEFINITION glfw_LIBRARIES)
get_directory_property(CMAKE_THREAD_LIBS_INIT DIRECTORY third_party/glfw-3.1.2 DEFINITION CMAKE_THREAD_LIBS_INIT)
get_directory_property(RT_LIBRARY DIRECTORY third_party/glfw-3.1.2 DEFINITION RT_LIBRARY)

target_link_libraries(Destroyer2D glfw)
target_link_libraries(Destroyer2D ${glfw_LIBRARIES})
target_link_libraries(Destroyer2D "${CMAKE_THREAD_LIBS_INIT}" "${RT_LIBRARY}")

target_link_libraries(Destroyer2D Box2D)
