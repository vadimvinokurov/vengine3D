cmake_minimum_required(VERSION 3.16)
project(vengine3D)

set(CMAKE_CXX_STANDARD 20)

add_subdirectory(glfw)

include_directories("./src")
include_directories("./glfw/include")

file(GLOB Sources "./src/*.cpp" "./src/*/*.cpp" "./src/*/*/*.cpp")

#if (MSVC)
#    add_compile_options(/W4 /WX)
#else()
#    add_compile_options(-Wall -Wextra -pedantic -Werror)
#endif()
add_executable(vengine3D ${Sources})
find_package(OpenGL REQUIRED)
include_directories( ${OPENGL_INCLUDE_DIRS} )
target_link_libraries(vengine3D glfw ${OPENGL_LIBRARIES})
target_precompile_headers(vengine3D PUBLIC src/stdlibraries.h)