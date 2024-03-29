cmake_minimum_required(VERSION 3.25)
cmake_policy(VERSION 3.25)
project(FCollada
    VERSION 3.5.99
    DESCRIPTION "Fork of Feeling Softwares FCollada"
    HOMEPAGE_URL "https://github.com/seragh/fcollada"
    LANGUAGES CXX
)

include(CTest)

if(NOT CMAKE_BUILD_TYPE)
    message(STATUS "No build type selected, defaulting to Debug")
    set(CMAKE_BUILD_TYPE "Debug")
endif()

if(NOT DEFINED CMAKE_POSITION_INDEPENDENT_CODE)
    set(CMAKE_POSITION_INDEPENDENT_CODE ON)
endif()


option(BUILD_SHARED_LIBS "Build FCollada as a shared library." OFF)
option(BUILD_UNICODE "Build FCollada in wide-character mode." OFF)
option(BUILD_TESTING "Build the test suite." OFF)
option(BUILD_DOCS "Build html docs using doxygen" OFF)
option(USE_FMT_HEADER_ONLY "Use header only fmt library" OFF)

option(FCOLLADA_ENABLE_COVERAGE "Build with coverage support." OFF)
mark_as_advanced(FCOLLADA_ENABLE_COVERAGE)

find_package(LibXml2 REQUIRED)
find_package(fmt REQUIRED)


if(${CMAKE_BUILD_TYPE} STREQUAL "Debug")
    add_definitions(-D_DEBUG)
endif()

if(WIN32)
    add_definitions(-DWIN32)
elseif(NOT APPLE)
    add_definitions(-DLINUX)
endif()

if(BUILD_UNICODE)
    add_definitions(-DUNICODE -D_UNICODE)
endif()

if(USE_FMT_HEADER_ONLY)
    add_definitions(-DFMT_HEADER_ONLY)
endif()


set(LIBRARY_NAME FCollada)
if(NOT BUILD_SHARED_LIBS AND NOT MSVC)
    set(LIBRARY_NAME "${LIBRARY_NAME}S")
endif()
if(BUILD_UNICODE)
    set(LIBRARY_NAME "${LIBRARY_NAME}U")
endif()
if(CMAKE_BUILD_TYPE STREQUAL "Debug" AND NOT MSVC)
    set(LIBRARY_NAME "${LIBRARY_NAME}D")
endif()

add_library(FCollada)
target_compile_features(FCollada PUBLIC cxx_std_17)
target_link_libraries(FCollada
    LibXml2::LibXml2
    fmt::fmt
    ${CMAKE_DL_LIBS})
set_target_properties(FCollada
    PROPERTIES
        OUTPUT_NAME "${LIBRARY_NAME}"
)
include(GenerateExportHeader)
generate_export_header(FCollada)

target_sources(FCollada
    PUBLIC
        FILE_SET HEADERS
        BASE_DIRS
            "${CMAKE_CURRENT_SOURCE_DIR}/FCollada"
            "${CMAKE_CURRENT_BINARY_DIR}"
        FILES
            ${CMAKE_CURRENT_BINARY_DIR}/fcollada_export.h
)


add_executable(FColladaTest)
set_target_properties(FColladaTest
    PROPERTIES
        EXCLUDE_FROM_ALL "$<NOT:$<BOOL:${BUILD_TESTING}>>"
)
target_link_libraries(FColladaTest FCollada)
target_include_directories(FColladaTest PRIVATE FCollada/FColladaTest)

add_custom_command(
    TARGET FColladaTest POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy
        ${CMAKE_CURRENT_SOURCE_DIR}/FCollada/FColladaTest/Samples/Eagle.DAE
        "$<TARGET_FILE_DIR:FColladaTest>"
    COMMAND ${CMAKE_COMMAND} -E copy
        ${CMAKE_CURRENT_SOURCE_DIR}/FCollada/FColladaTest/Samples/TestSphere.dae
        "$<TARGET_FILE_DIR:FColladaTest>"
)

add_test(NAME runTests
    COMMAND FColladaTest
    WORKING_DIRECTORY "$<TARGET_FILE_DIR:FColladaTest>"
)

# TODO: clumsy approch without changing the test runner
set(TEST_LOG "$<SHELL_PATH:$<TARGET_FILE_DIR:FColladaTest>/FColladaTestLog.txt>")
if(WIN32)
    add_test(NAME checkTestLog
        COMMAND powershell type ${TEST_LOG}
    )
else()
    add_test(NAME checkTestLog
        COMMAND cat ${TEST_LOG}
    )
endif()
set_tests_properties(checkTestLog PROPERTIES
  FAIL_REGULAR_EXPRESSION "failed"
)


if(FCOLLADA_ENABLE_COVERAGE)
    include(cmake/coverage.cmake)
endif()


if(BUILD_DOCS)
    find_package(Doxygen REQUIRED)
    doxygen_add_docs(
        doxygen
        ALL
        ${PROJECT_SOURCE_DIR}
        COMMENT "Generate html pages")
endif()


install(TARGETS FCollada
    FILE_SET HEADERS
)

configure_file("fcollada.pc.in" "fcollada.pc" @ONLY)
install(FILES "${CMAKE_CURRENT_BINARY_DIR}/fcollada.pc"
    DESTINATION "lib/pkgconfig")


add_subdirectory(FCollada)
add_subdirectory(FColladaPlugins)
