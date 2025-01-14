cmake_minimum_required(VERSION 3.15)
project(DictionaryOverlay VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

#set(CMAKE_PREFIX_PATH "C:/vcpkg/installed/x64-windows/share" ${CMAKE_PREFIX_PATH})

option(BUILD_TESTS "Build tests" ON)
option(USE_GPU "Enable GPU support" ON)

if(DEFINED CMAKE_TOOLCHAIN_FILE)
    message(STATUS "Using vcpkg toolchain file: ${CMAKE_TOOLCHAIN_FILE}")
else()
    set(CMAKE_TOOLCHAIN_FILE "C:/vcpkg/scripts/buildsystems/vcpkg.cmake"
            CACHE STRING "Vcpkg toolchain file")
endif()

find_package(Qt6 REQUIRED COMPONENTS Core Gui Widgets Concurrent)
set(OpenCV_DIR "C:/vcpkg/installed/x64-windows/share/opencv4")
find_package(OpenCV REQUIRED)
find_package(Tesseract REQUIRED)
find_package(SQLite3 REQUIRED)
find_package(TIFF MODULE REQUIRED)

add_subdirectory(${CMAKE_SOURCE_DIR}/src)

set(SOURCES
    src/main.cpp
    src/ui/main_window.ui
    src/ui/overlay_window.ui
    src/ui/settings_dialog.ui
    ../resources/resources.qrc
)
#add_executable(DictionaryOverlay ${SOURCES})

target_include_directories(DictionaryOverlay PRIVATE
        ${CMAKE_SOURCE_DIR}/include
        ${TIFF_INCLUDE_DIRS}
)

target_link_libraries(DictionaryOverlay PRIVATE
        Qt6::Core
        Qt6::Gui
        Qt6::Widgets
        Qt6::Concurrent
        opencv
        Tesseract
        SQLite3
        ${TIFF_LIBRARIES}
)

if(USE_GPU)
    target_compile_definitions(DictionaryOverlay PRIVATE USE_GPU)
endif()

if(BUILD_TESTS)
    enable_testing()
    add_subdirectory(tests)
endif()

install(DIRECTORY ${CMAKE_SOURCE_DIR}/resources
        DESTINATION ${CMAKE_INSTALL_PREFIX}/resources)

include(CPack)
