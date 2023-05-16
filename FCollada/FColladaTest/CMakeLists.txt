target_sources(FColladaTest
	PRIVATE
        FCTest.cpp
        FCTestAnimation.cpp
        FCTestArchiving.cpp
        FCTestController.cpp
        FCTestGeometryPolygonsTools.cpp
        FCTestParameters.cpp
        FCTestSceneGraph.cpp
        StdAfx.cpp
        StdAfx.h
)


add_subdirectory(FCTestAssetManagement)
add_subdirectory(FCTestExportImport)
add_subdirectory(FCTestXRef)
