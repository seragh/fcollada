target_sources(FCollada
    PRIVATE
        DLLEntry.cpp
        FCollada.cpp
        FColladaPlugin.cpp
        StdAfx.cpp
        StdAfx.h
    PUBLIC FILE_SET HEADERS
    FILES
        FCollada.h
        FColladaPlugin.h
)

add_subdirectory(FCDocument)
add_subdirectory(FColladaTest)
add_subdirectory(FMath)
add_subdirectory(FUtils)
