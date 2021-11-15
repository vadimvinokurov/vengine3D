cmake_minimum_required(VERSION 3.16)
project(vengine3D)

set(CMAKE_CXX_STANDARD 20)

add_subdirectory(glfw)

include_directories("./src")
include_directories("./glfw/include")

file(GLOB Sources_src "./src/*.cpp")
file(GLOB Sources_subdir "./src/*/*.cpp")

set (SOURCE_FILES ${Sources_src} ${Sources_subdir})



add_executable(vengine3D ${SOURCE_FILES})
target_link_libraries(vengine3D glfw -lopengl32)