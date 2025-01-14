find_package(GTest REQUIRED)

#set(TEST_FILES
#    test_screenshot_managed.cpp
#    test_ocr_engine.cpp
#    test_dictionary_service.cpp
#    test_thread_pool.cpp
#)

#add_executable(DictionaryOverlayTests
#    ${TEST_FILES}
#)

#target_link_libraries(DictionaryOverlayTests
#    PRIVATE
#        GTest::GTest
#        GTest::Main
#        Qt6::Core
#        Qt6::Gui
#        Qt6::Widgets
#        Qt6::Concurrent
#        ${OpenCV_LIBS}
#        ${PaddleOCR_LIBS}
#        SQLite::SQLite3
#)

#add_test(NAME unit_tests COMMAND unit_tests)