cmake_minimum_required(VERSION 3.13)

#AnalyticHierarchyProcess
project(AHP)

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_PREFIX_PATH "C:/Qt/5.12.1/mingw73_64/lib/cmake/")

find_package(Qt5Core REQUIRED)
find_package(Qt5Gui REQUIRED)
find_package(Qt5Widgets REQUIRED)


set(SOURCES
        src/main.cpp
        src/mainwindow.cpp
        src/dialog.cpp
        src/compute/ahp.cpp
        src/delegate/spinboxdelegate.cpp
        src/editableTreeView/treeitem.cpp
        src/editableTreeView/treemodel.cpp)

set(HEADERS
        src/mainwindow.h
        src/dialog.h
        src/compute/ahp.h
        src/delegate/spinboxdelegate.h
        src/editableTreeView/treeitem.h
        src/editableTreeView/treemodel.h)

set(FORMS
        src/dialog.ui
        src/mainwindow.ui)


set(RESOURCES
        res.qrc)

add_executable(AHP ${SOURCES} ${HEADERS} ${FORMS} ${RESOURCES})

target_link_libraries(AHP Qt5::Core)
target_link_libraries(AHP Qt5::Gui)
target_link_libraries(AHP Qt5::Widgets)