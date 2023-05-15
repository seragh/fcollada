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

find_package(LibXml2 REQUIRED)

file(GLOB SOURCE_FILES
    "../FColladaPlugins/FArchiveXML/*.cpp"
)

include_directories(${CMAKE_CURRENT_SOURCE_DIR})
add_library(FCollada ${SOURCE_FILES})
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

target_sources(FCollada
    PRIVATE
        DLLEntry.cpp
        FCollada.cpp
        FColladaPlugin.cpp
        StdAfx.cpp
        StdAfx.h
    PUBLIC FILE_SET HEADERS
    BASE_DIRS "${PROJECT_SOURCE_DIR}"
    FILES
        FCollada.h
        FColladaPlugin.h
)


if(BUILD_TESTS)
    # TODO: Some tests are built as part of the library.
    # TODO: Copy required assets for running tests.
    # TODO: Integrate tests with ctest
    file(GLOB TEST_SOURCE_FILES
        "FColladaTest/FCTestExportImport/*.cpp"
        "FColladaTest/FCTestXRef/*.cpp"
        "FColladaTest/FCTestAssetManagement/*.cpp"
        "FColladaTest/*.cpp"
    )

    add_executable(FColladaTest ${TEST_SOURCE_FILES})
    target_link_libraries(FColladaTest FCollada)
endif()


install(TARGETS FCollada
    FILE_SET HEADERS
)

add_subdirectory(FCDocument)
add_subdirectory(FMath)
add_subdirectory(FUtils)
