QT += widgets sql

CONFIG += c++17

TEMPLATE = app
TARGET = ejercicio09_qt

SOURCES += \
    admindb.cpp \
    login.cpp \
    main.cpp \
    pintura.cpp

HEADERS += \
    admindb.h \
    login.h \
    pintura.h

FORMS += \
    login.ui

RESOURCES += \
    recursos.qrc

DISTFILES += \
    flores.png
