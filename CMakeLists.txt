cmake_minimum_required(VERSION 3.1.0)

project(helloworld)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

if(CMAKE_VERSION VERSION_LESS "3.7.0")
    set(CMAKE_INCLUDE_CURRENT_DIR ON)
endif()

find_package(Qt5 COMPONENTS Core Gui Widgets REQUIRED)

add_executable(helloworld
    main.cpp
     "lex_analyzer.cpp" 
     "lex_analyzer.h" 
     "parser.cpp" 
     "postfix_handler.cpp"
     "postfix_handler.h"
     "interpreter.cpp" 
     "interpreter.h" 
     "common.h" 
     "common.cpp"
 "interpreter_window.cpp")

target_link_libraries(helloworld Qt5::Widgets)


add_custom_command(
    TARGET "helloworld" POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E remove_directory "${CMAKE_CURRENT_BINARY_DIR}/resources"
)

add_custom_command(
    TARGET "helloworld" POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    "${CMAKE_CURRENT_SOURCE_DIR}/resources"
    "${CMAKE_CURRENT_BINARY_DIR}/resources"
)

if (WIN32)
    # Чтобы убрать консоль
    #set_property(TARGET "helloworld" PROPERTY WIN32_EXECUTABLE true)

    # Копирование DLL-файлов библиотеки Qt в выходную директорию
    add_custom_command(
        TARGET "helloworld" POST_BUILD
        COMMAND ${Qt5_DIR}/../../../bin/windeployqt $<TARGET_FILE:helloworld>
    )
endif (WIN32)