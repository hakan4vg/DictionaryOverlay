cmake_minimum_required(VERSION 3.15)
project(DictionaryOverlay VERSION 1.0.0 LANGUAGES CXX)

if(NOT EXISTS H:/code/DictionaryOverlay/build/generators/conan_toolchain.cmake)
    message(FATAL_ERROR "The file conantoolchain.cmake doesn't exist. Please run 'conan install . --output-folder=build --build=missing' first")
endif()
include("H:/code/DictionaryOverlay/build/generators/conan_toolchain.cmake")

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

option(BUILD_TESTS "Build tests" ON)
option(USE_GPU "Enable GPU support" ON)

find_package(Qt6 REQUIRED COMPONENTS
        Core
        Gui
        Widgets
        Concurrent
)
find_package(OpenCV REQUIRED)
find_package(Tesseract REQUIRED)
find_package(SQLite3 REQUIRED)

include_directories(${CMAKE_SOURCE_DIR}/include)

add_subdirectory(../DictionaryOverlay/src)

if(BUILD_TESTS)
    enable_testing()
    add_subdirectory(tests)
endif()

install(DIRECTORY ${CMAKE_SOURCE_DIR}/resources
        DESTINATION ${CMAKE_INSTALL_PREFIX}/resources)

include(CPack)

if(USE_GPU)
    target_compile_definitions(DictionaryOverlay PRIVATE USE_GPU)
endif()


