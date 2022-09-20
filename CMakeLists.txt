cmake_minimum_required(VERSION 3.16)
project(vengine3D)

set(CMAKE_CXX_STANDARD 17)

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
        gtest/cci.20210126
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
include_directories("EngineSource")
include_directories("GameSources")

add_compile_definitions(ECS_DEBUG)


file(GLOB EngineSource "EngineSource/*.cpp"
        "EngineSource/Render/*.cpp"
        "EngineSource/Core/*.cpp"
        "EngineSource/Core/Objects/*.cpp"
        "EngineSource/Core/Utils/*.cpp"
        "EngineSource/Core/Memory/*.cpp"
        "EngineSource/Core/Memory/Allocator/*.cpp"
        "EngineSource/Systems/*.cpp"
        "EngineSource/Components/*.cpp")

file(GLOB GameSource "GameSource/*.cpp")
file(GLOB EngineTests "EngineTests/*.cpp")


add_executable(${PROJECT_NAME} ${EngineSource} ${GameSource})
target_link_libraries(${PROJECT_NAME} PRIVATE imgui::imgui glfw glad::glad spdlog::spdlog stb_image assimp)

find_package(GTest)

# For Windows: Prevent overriding the parent project's compiler/linker settings
add_executable(${PROJECT_NAME}_test ${EngineSource} ${EngineTests})
target_link_libraries(${PROJECT_NAME}_test PRIVATE imgui::imgui glfw glad::glad spdlog::spdlog stb_image assimp GTest::gtest GTest::gtest_main)

if (MSVC)
    set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
    #target_compile_options(${PROJECT_NAME}_test PRIVATE /W4 /WX)
else ()
    #target_compile_options(${PROJECT_NAME}_test PRIVATE -Wall -Wextra -pedantic -Werror)
endif ()

