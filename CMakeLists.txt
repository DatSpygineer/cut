cmake_minimum_required(VERSION 3.20)

project(cut C)

enable_testing()

set(CMAKE_C_STANDARD 11)

add_library(cut STATIC cut.c)
target_include_directories(cut PUBLIC ${CMAKE_CURRENT_LIST_DIR})

macro(add_unit_test MODULE_NAME)
    set(options)
    set(oneValueArgs)
    set(multiValueArgs SOURCES INCLUDES LIBRARIES DEFINES COMPILE_OPTIONS)
    cmake_parse_arguments(UT
        "${options}" "${oneValueArgs}" "${multiValueArgs}"
        ${ARGN}
    )

    set(TARGET_NAME ${MODULE_NAME}_test)

    if (NOT UT_SOURCES)
        message(FATAL_ERROR "No source file specified!")
    endif()

    if (CUT_LINK_LIBRARY)
        set(CUT_LIBRARY cut)
    else()
        set(CUT_LIBRARY "")
    endif()

    add_executable(${TARGET_NAME} ${UT_SOURCES})
    target_link_libraries(${TARGET_NAME} PRIVATE ${CUT_LIBRARY} cut ${UT_LIBRARIES})
    if (NOT UT_INCLUDES)
        target_include_directories(${TARGET_NAME} PRIVATE ${UT_INCLUDES})
    endif()
    if (NOT UT_DEFINES)
        target_compile_definitions(${TARGET_NAME} PRIVATE ${UT_DEFINES})
    endif()
    if (NOT UT_COMPILE_OPTIONS)
        target_compile_options(${TARGET_NAME} PRIVATE ${UT_COMPILE_OPTIONS})
    endif()

    if (CMAKE_TESTING_ENABLED)
        target_compile_definitions(${TARGET_NAME} PUBLIC -DCUT_DISABLE_TEXT_COLORS)
    endif()

    add_test(test_${MODULE_NAME} ${TARGET_NAME})
endmacro()

add_unit_test(cut
    SOURCES cut_example.c
)