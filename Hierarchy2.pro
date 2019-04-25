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
    input.cpp


HEADERS += \
        mainwindow.h \
        dialog.h \
        spinboxdelegate.h \
        alg_ahp.h \
    input.h

FORMS += \
        mainwindow.ui \
        dialog.ui \
    input.ui

RESOURCES += \
        res.qrc


