#-------------------------------------------------
#
# AnalyticHierarchyProcess
#
# Project created by QtCreator 2019-03-27T13:56:17
#
#-------------------------------------------------

QT += core gui widgets


TARGET = ahp
TEMPLATE = app


CONFIG += c++17


SOURCES += \
        src/main.cpp \
        src/mainwindow.cpp \
        src/dialog.cpp \
        src/compute/ahp.cpp \
        src/delegate/spinboxdelegate.cpp \
        src/editableTreeView/treeitem.cpp \
        src/editableTreeView/treemodel.cpp


HEADERS += \
        src/mainwindow.h \
        src/dialog.h \
        src/compute/ahp.h \
        src/delegate/spinboxdelegate.h \
        src/editableTreeView/treeitem.h \
        src/editableTreeView/treemodel.h


FORMS += \
        src/dialog.ui \
        src/mainwindow.ui


RESOURCES += \
        res.qrc

RC_FILE = res/myapp.rc
win32:RC_ICONS = $$PWD/res/graph.ico