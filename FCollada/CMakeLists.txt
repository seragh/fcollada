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


add_subdirectory(FCDocument)
add_subdirectory(FMath)
add_subdirectory(FUtils)
