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


add_subdirectory(FCDocument)
add_subdirectory(FMath)
add_subdirectory(FUtils)

if(BUILD_TESTS)
    add_subdirectory(FColladaTest)
endif()
