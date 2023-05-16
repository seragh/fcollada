cmake_minimum_required(VERSION 3.25)
cmake_policy(VERSION 3.25)
project(FCollada
    VERSION 3.5.99
    DESCRIPTION "Fork of Feeling Softwares FCollada"
    HOMEPAGE_URL "https://github.com/seragh/fcollada"
    LANGUAGES CXX
)

if(NOT CMAKE_BUILD_TYPE)
    message(STATUS "No build type selected, defaulting to Debug")
    set(CMAKE_BUILD_TYPE "Debug")
endif()

option(BUILD_SHARED_LIBS "Build FCollada as a shared library." OFF)
option(BUILD_UNICODE "Build FCollada in wide-character mode." OFF)
option(BUILD_TESTS "Build the test suite." OFF)
option(BUILD_DOCS "Build html docs using doxygen" OFF)
# TODO: Support vcpkg or similar
#option(USE_SYSTEM_LIBXML2 "Use system provided libxml2" ON)


find_package(LibXml2 REQUIRED)


set(LIBRARY_NAME FCollada)

if(BUILD_SHARED_LIBS)
    add_definitions(-DFCOLLADA_DLL)
elseif(NOT MSVC)
    set(LIBRARY_NAME "${LIBRARY_NAME}S")
endif()

if(BUILD_UNICODE)
    add_definitions(-DUNICODE -D_UNICODE)
    set(LIBRARY_NAME "${LIBRARY_NAME}U")
endif()

if(NOT BUILD_TESTS)
    add_definitions(-DRETAIL)
endif()

if(${CMAKE_SYSTEM_NAME} MATCHES "Windows")
    add_definitions(-DWIN32)
endif()

if(${CMAKE_SYSTEM_NAME} MATCHES "Linux")
    add_definitions(-DLINUX)
endif()

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/FCollada)
add_library(FCollada)
target_link_libraries(FCollada
    LibXml2::LibXml2
    ${CMAKE_DL_LIBS})
set_target_properties(FCollada
    PROPERTIES
        OUTPUT_NAME "${LIBRARY_NAME}"
)
if(NOT MSVC)
    set_target_properties(FCollada
        PROPERTIES
            OUTPUT_NAME_DEBUG ${LIBRARY_NAME}D
    )
endif()


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


set(DEST_DIR "${CMAKE_INSTALL_PREFIX}")

configure_file("fcollada.pc.in" "fcollada.pc" @ONLY)
install(FILES "${CMAKE_CURRENT_BINARY_DIR}/fcollada.pc"
    DESTINATION "lib/pkgconfig")

add_subdirectory(FCollada)
add_subdirectory(FColladaPlugins)
