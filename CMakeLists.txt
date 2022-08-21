cmake_minimum_required(VERSION 3.16)
project(vengine3D)

set(CMAKE_CXX_STANDARD 17)

#if (MSVC)
#    add_compile_options(/W4 /WX)
#else ()
#    add_compile_options(-Wall -Wextra -pedantic -Werror)
#endif ()

if (NOT EXISTS "${CMAKE_BINARY_DIR}/conan.cmake")
    message(STATUS "Downloading conan.cmake from https://github.com/conan-io/cmake-conan")
    file(DOWNLOAD "https://raw.githubusercontent.com/conan-io/cmake-conan/master/conan.cmake"
            "${CMAKE_BINARY_DIR}/conan.cmake")
endif ()

include(${CMAKE_BINARY_DIR}/conan.cmake)

conan_cmake_configure(
        REQUIRES
        physx/4.1.1
        glfw/3.3.7
        imgui/1.88
        glad/0.1.36
        spdlog/1.10.0
        GENERATORS
        cmake_find_package
        CMakeDeps
        CMakeToolchain
)
conan_cmake_autodetect(settings)
conan_cmake_install(
        PATH_OR_REFERENCE .
        BUILD missing
        REMOTE conancenter
        SETTINGS ${settings}
)

find_package(glad)
find_package(glfw3)
find_package(imgui)
find_package(spdlog)

set(BUILD_SHARED_LIBS OFF)
add_subdirectory(External/assimp)
add_subdirectory(External/stb_image)


include_directories("External/stb_image")
include_directories("External/assimp")
include_directories("Sources")

add_compile_definitions(ECS_DEBUG)


file(GLOB Sources "Sources/*.cpp"
        "Sources/Render/*.cpp"
        "Sources/Core/*.cpp"
        "Sources/Core/Objects/*.cpp"
        "Sources/Core/Utils/*.cpp"
        "Sources/Core/Memory/*.cpp"
        "Sources/Core/Memory/Allocator/*.cpp"
        "Sources/Systems/*.cpp"
        "Sources/Components/*.cpp")


add_executable(${PROJECT_NAME} ${Sources})
target_link_libraries(${PROJECT_NAME} imgui::imgui glfw glad::glad spdlog::spdlog stb_image assimp)