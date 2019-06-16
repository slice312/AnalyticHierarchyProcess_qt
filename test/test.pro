QT += testlib core
QT -= gui



CONFIG += c++17
#CONFIG += qt console warn_on depend_includepath testcase
CONFIG += testcase

TEMPLATE = app

HEADERS += \
        ../main/src/compute/ahp.h

SOURCES += \
        ../main/src/compute/ahp.cpp \
        test_algorithmahp.cpp
