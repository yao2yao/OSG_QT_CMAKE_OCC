cmake_minimum_required (VERSION 3.8)

# project
set (PROJECT_NAME "QT_CMAKE_OSG_OCC")
project (${PROJECT_NAME})

# allow to mkdir
set_property(GLOBAL PROPERTY USE_FOLDERS ON)
# modify default project's name
set_property(GLOBAL PROPERTY PREDEFINED_TARGETS_FOLDER "CMakeTargets")

# c++ 11
set (CMAKE_CXX_STANDARD 11)

# osg
set (OSG_DIR ${CMAKE_CURRENT_SOURCE_DIR}/thirdpart/OpenSceneGraph-3.6.5-VC2019-64-Debug)
include_directories (${OSG_DIR}/include)
link_directories (${OSG_DIR}/lib)

#occ
set (OCC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/thirdpart/OpenCASCADE-7.7.0-vc14-64/win64/vc14)
include_directories (${OCC_DIR}/inc)
link_directories (${OCC_DIR}/lib)

#tbb
set (TBB_DIR ${CMAKE_CURRENT_SOURCE_DIR}/thirdpart/tbb_2021.5-vc14-64)
include_directories (${TBB_DIR}/include)
link_directories (${TBB_DIR}/lib)

# qt
set (CMAKE_AUTOMOC ON)
set (CMAKE_AUTORCC ON)
set (CMAKE_AUTOUIC ON)

set (CMAKE_PREFIX_PATH ${CMAKE_CURRENT_SOURCE_DIR}/thirdpart/qt5.15/msvc2019_64)
message ("pre_path: ${CMAKE_PREFIX_PATH}")

# auto find QT5
find_package (Qt5 COMPONENTS Widgets Core Gui OpenGL REQUIRED)

include_directories (${CMAKE_CURRENT_BINARY_DIR})

# src file
set (SRC_FILE
	src/main.cpp
	src/QTTEST.cpp
)

# header file
set (HEADER_FILE
	src/QTTEST.h
)

# ui file
set (UI_FILE
	src/QTTEST.ui
)

# resource file
set (QRC_FILE
	src/QTTEST.qrc
)

add_executable (${PROJECT_NAME}
	${SRC_FILE}
	${HEADER_FILE}
	${UI_FILE}
	${QRC_FILE}
)

# qt link
target_link_libraries (${PROJECT_NAME} Qt5::Widgets Qt5::Core Qt5::Gui Qt5::OpenGL)
# osg link
target_link_libraries(${PROJECT_NAME} debug OpenThreadsd)
target_link_libraries(${PROJECT_NAME} debug osgd)
target_link_libraries(${PROJECT_NAME} debug osgViewerd)
target_link_libraries(${PROJECT_NAME} debug osgDBd)
target_link_libraries(${PROJECT_NAME} debug osgGAd)
target_link_libraries(${PROJECT_NAME} debug osgUtild)
target_link_libraries(${PROJECT_NAME} debug osgQt5d)

# occ link
target_link_libraries(${PROJECT_NAME} debug TKPrim)
target_link_libraries(${PROJECT_NAME} debug TKBRep)
target_link_libraries(${PROJECT_NAME} debug TKMesh)
target_link_libraries(${PROJECT_NAME} debug TKMath)
target_link_libraries(${PROJECT_NAME} debug TKernel)
target_link_libraries(${PROJECT_NAME} debug TKTopAlgo)

set_target_properties (${PROJECT_NAME} PROPERTIES 
    VS_DEBUGGER_ENVIRONMENT "PATH=${CMAKE_CURRENT_SOURCE_DIR}/thirdpart/qt5.15/msvc2019_64/bin;${OSG_DIR}/bin;${OCC_DIR}/bin;${TBB_DIR}/bin"
)

set_target_properties(${PROJECT_NAME} PROPERTIES FOLDER "Qt_Osg_Occ")

# 拷贝data到运行目录
# file (COPY ${CMAKE_CURRENT_SOURCE_DIR}/data DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/)