cmake_minimum_required(VERSION 3.16)
project(vengine3D)

set(CMAKE_CXX_STANDARD 20)

add_subdirectory(glfw)

include_directories("./src")
include_directories("./glfw/include")

file(GLOB Sources "./src/*.cpp" "./src/*/*.cpp" "./src/*/*/*.cpp")


add_executable(vengine3D ${Sources})
target_link_libraries(vengine3D glfw -lopengl32)