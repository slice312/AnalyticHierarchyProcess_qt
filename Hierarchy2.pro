#-------------------------------------------------
#
# Project created by QtCreator 2019-03-27T13:56:17
#
#-------------------------------------------------

QT       += core gui widgets



TARGET = Hierarchy2
TEMPLATE = app


CONFIG += c++17


SOURCES += \
        main.cpp \
        mainwindow.cpp \
        dialog.cpp \
        spinboxdelegate.cpp \
        console.cpp \
        alg_ahp.cpp \
        editableTreeView\treeitem.cpp \
        editableTreeView\treemodel.cpp


HEADERS += \
        mainwindow.h \
        dialog.h \
        spinboxdelegate.h \
        alg_ahp.h \
        editableTreeView\treeitem.h \
        editableTreeView\treemodel.h


FORMS += \
        mainwindow.ui

RESOURCES += \
        res.qrc


