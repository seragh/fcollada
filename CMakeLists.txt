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

if(BUILD_DOCS)
    find_package(Doxygen REQUIRED)
    doxygen_add_docs(
        doxygen
        ALL
        ${PROJECT_SOURCE_DIR}
        COMMENT "Generate html pages")
endif()

add_subdirectory(FCollada)

set(DEST_DIR "${CMAKE_INSTALL_PREFIX}")
CONFIGURE_FILE("fcollada.pc.in" "fcollada.pc" @ONLY)

install(FILES fcollada.pc DESTINATION lib/pkgconfig)
