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


find_package(LibXml2 REQUIRED)


if(WIN32)
    add_definitions(-DWIN32)
elseif(NOT APPLE)
    add_definitions(-DLINUX)
endif()

if(BUILD_SHARED_LIBS)
    add_definitions(-DFCOLLADA_DLL)
endif()

if(BUILD_UNICODE)
    add_definitions(-DUNICODE -D_UNICODE)
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

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/FCollada)
add_library(FCollada)
target_compile_features(FCollada PUBLIC cxx_std_17)
target_link_libraries(FCollada
    LibXml2::LibXml2
    ${CMAKE_DL_LIBS})
set_target_properties(FCollada
    PROPERTIES
        OUTPUT_NAME "${LIBRARY_NAME}"
)


# TODO: Some tests are built as part of the library.
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


if(BUILD_DOCS)
    find_package(Doxygen REQUIRED)
    doxygen_add_docs(
        doxygen
        ALL
        ${PROJECT_SOURCE_DIR}
        COMMENT "Generate html pages")
endif()


# Make the fileset known to the parser
target_sources(FCollada
    PUBLIC FILE_SET HEADERS
    BASE_DIRS ${PROJECT_SOURCE_DIR}
)
install(TARGETS FCollada
    FILE_SET HEADERS
)


configure_file("fcollada.pc.in" "fcollada.pc" @ONLY)
install(FILES "${CMAKE_CURRENT_BINARY_DIR}/fcollada.pc"
    DESTINATION "lib/pkgconfig")

add_subdirectory(FCollada)
add_subdirectory(FColladaPlugins)
