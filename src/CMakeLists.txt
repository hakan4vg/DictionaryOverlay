set(SOURCE_FILES
    main.cpp
    screenshot/screenshot_manager.cpp
    ocr/ocr_engine.cpp
    dictionary/dictionary_service.cpp
    ui/main_window.cpp
    ui/overlay_window.cpp
    settings/settings_manager.cpp
    utils/utils.cpp
)

set(HEADER_FILES
    ../include/screenshot/screenshot_manager.h
    ../include/ocr/ocr_engine.h
    ../include/dictionary/dictionary_service.h
    ../include/ui/main_window.h
    ../include/ui/overlay_window.h
    ../include/settings/settings_manager.h
    ../include/utils/utils.h
)

set(UI_FILES
    ui/main_window.ui
    ui/overlay_window.ui
    ui/settings_dialog.ui
)

qt_wrap_ui(UI_HEADERS ${UI_FILES})

set(RESOURCE_FILES
    ../resources/resources.qrc
)
qt_add_resources(RESOURCE_FILES_CPP ${RESOURCE_FILES})

add_executable(DictionaryOverlay
    ${SOURCE_FILES}
    ${HEADER_FILES}
    ${UI_HEADERS}
    ${RESOURCE_FILES_CPP}
)

target_link_libraries(DictionaryOverlay
    PRIVATE
        Qt6::Core
        Qt6::Gui
        Qt6::Widgets
        Qt6::Concurrent
        ${OpenCV_LIBS}
        ${Tesseract_LIBS}
        SQLite::SQLite3
)

target_include_directories(DictionaryOverlay
    PRIVATE
        ${CMAKE_SOURCE_DIR}/include
        ${OpenCV_INCLUDE_DIRS}
        ${Tesseract_INCLUDE_DIRS}
)