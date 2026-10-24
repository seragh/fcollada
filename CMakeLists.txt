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


option(FCOLLADA_SHARED "Build FCollada as a shared library." OFF)
option(FCOLLADA_BUILD_UNICODE "Build FCollada in wide-character mode." OFF)
option(FCOLLADA_BUILD_TESTING "Build the test suite." OFF)
option(FCOLLADA_BUILD_DOCS "Build html docs using doxygen" OFF)
option(FCOLLADA_USE_FMT_HEADER_ONLY "Use header only fmt library" OFF)
option(FCOLLADA_ENABLE_COVERAGE "Build with coverage support." OFF)
option(FCOLLADA_INSTALL "Enables the installation target" ON)

mark_as_advanced(FCOLLADA_ENABLE_COVERAGE)

find_package(LibXml2 REQUIRED)
find_package(fmt REQUIRED)


if(${CMAKE_BUILD_TYPE} STREQUAL "Debug")
    add_definitions(-D_DEBUG)
endif()

if(FCOLLADA_BUILD_UNICODE)
    add_definitions(-DUNICODE -D_UNICODE)
endif()

if(FCOLLADA_USE_FMT_HEADER_ONLY)
    add_definitions(-DFMT_HEADER_ONLY)
endif()

if(FCOLLADA_SHARED)
    set(FCOLLADA_LIB_TYPE SHARED)
else()
    set(FCOLLADA_LIB_TYPE STATIC)
endif()

if(NOT MSVC)
    # clang-14 enabled it by default and on aarch64 this results
    # in a loss of precision enough to fail the tests.
    add_compile_options(-ffp-contract=off)
endif()


set(LIBRARY_NAME ${PROJECT_NAME})
if(NOT FCOLLADA_SHARED AND NOT MSVC)
    set(LIBRARY_NAME "${LIBRARY_NAME}S")
endif()
if(FCOLLADA_BUILD_UNICODE)
    set(LIBRARY_NAME "${LIBRARY_NAME}U")
endif()
if(CMAKE_BUILD_TYPE STREQUAL "Debug" AND NOT MSVC)
    set(LIBRARY_NAME "${LIBRARY_NAME}D")
endif()

add_library(${PROJECT_NAME} ${FCOLLADA_LIB_TYPE})
target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_17)
target_link_libraries(${PROJECT_NAME}
    LibXml2::LibXml2
    fmt::fmt
    ${CMAKE_DL_LIBS})
set_target_properties(${PROJECT_NAME}
    PROPERTIES
        OUTPUT_NAME "${LIBRARY_NAME}"
)
target_compile_definitions(${PROJECT_NAME} PRIVATE $<$<AND:$<CXX_COMPILER_ID:MSVC>,$<NOT:$<BOOL:${FCOLLADA_BUILD_UNICODE}>>>:FMT_UNICODE=0>)
target_compile_options(${PROJECT_NAME} PRIVATE $<$<AND:$<CXX_COMPILER_ID:MSVC>,$<BOOL:${FCOLLADA_BUILD_UNICODE}>>:/utf-8>)
include(GenerateExportHeader)
generate_export_header(${PROJECT_NAME})

target_sources(${PROJECT_NAME}
    PUBLIC
        FILE_SET HEADERS
        BASE_DIRS
            "${CMAKE_CURRENT_SOURCE_DIR}/FCollada"
            "${CMAKE_CURRENT_BINARY_DIR}"
        FILES
            ${CMAKE_CURRENT_BINARY_DIR}/fcollada_export.h
)


add_executable(${PROJECT_NAME}Test)
set_target_properties(${PROJECT_NAME}Test
    PROPERTIES
        EXCLUDE_FROM_ALL "$<NOT:$<BOOL:${FCOLLADA_BUILD_TESTING}>>"
)
target_link_libraries(${PROJECT_NAME}Test ${PROJECT_NAME})
target_include_directories(${PROJECT_NAME}Test PRIVATE FCollada/FColladaTest)

add_custom_command(
    TARGET ${PROJECT_NAME}Test POST_BUILD
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
if(WIN32 AND NOT MINGW)
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


if(FCOLLADA_BUILD_DOCS)
    find_package(Doxygen REQUIRED)
    doxygen_add_docs(doxygen
        "${CMAKE_SOURCE_DIR}/FCollada"
        "${CMAKE_SOURCE_DIR}/FColladaPlugins"
        "${CMAKE_SOURCE_DIR}/FColladaTools"
        ALL
        COMMENT "Generate html pages")
endif()

if(FCOLLADA_INSTALL)
    include(GNUInstallDirs)
    install(TARGETS ${PROJECT_NAME}
        EXPORT ${PROJECT_NAME}Targets
        FILE_SET HEADERS
    )
    configure_file("fcollada.pc.in" "fcollada.pc" @ONLY)
    install(FILES "${CMAKE_CURRENT_BINARY_DIR}/fcollada.pc"
            DESTINATION "lib/pkgconfig"
    )
    # CMake find_package support
    include(CMakePackageConfigHelpers)
    set(FCOLLADA_CONFIG_FILE_NAME ${PROJECT_NAME}Config)
    # CMake configuration files installation directory
    set(FCOLLADA_TARGET_INSTALL_CMAKEDIR "${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}")

    write_basic_package_version_file(
        "${CMAKE_CURRENT_BINARY_DIR}/${FCOLLADA_CONFIG_FILE_NAME}Version.cmake"
        VERSION ${PROJECT_VERSION}
        COMPATIBILITY SameMajorVersion
    )
    # Configures the meta-file FColladaConfig.cmake.in to replace variables with paths/values/etc.
    configure_package_config_file("cmake/${FCOLLADA_CONFIG_FILE_NAME}.cmake.in"
        "${CMAKE_CURRENT_BINARY_DIR}/${FCOLLADA_CONFIG_FILE_NAME}.cmake"
        INSTALL_DESTINATION "${FCOLLADA_TARGET_INSTALL_CMAKEDIR}"
    )

    install(EXPORT ${PROJECT_NAME}Targets
        # Puts the targets into the FCollada namespace
        # So this makes FCollada::FCollada linkable after doing find_package(FCollada)
        NAMESPACE ${PROJECT_NAME}::
        DESTINATION ${FCOLLADA_TARGET_INSTALL_CMAKEDIR}
    )
    install(FILES
        "${CMAKE_CURRENT_BINARY_DIR}/${FCOLLADA_CONFIG_FILE_NAME}.cmake"
        "${CMAKE_CURRENT_BINARY_DIR}/${FCOLLADA_CONFIG_FILE_NAME}Version.cmake"
        DESTINATION ${FCOLLADA_TARGET_INSTALL_CMAKEDIR}
    )
endif()

add_subdirectory(FCollada)
add_subdirectory(FColladaPlugins)
