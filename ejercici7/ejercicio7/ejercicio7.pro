QT += widgets network

CONFIG += c++17
TEMPLATE = app
TARGET = ejercicio7

SOURCES += \
    src/main.cpp \
    src/appcontroller.cpp \
    src/appconfig.cpp \
    src/climawidget.cpp \
    src/logger.cpp \
    src/loginwindow.cpp \
    src/mainwindow.cpp \
    src/networkservice.cpp \
    src/pantalla.cpp

HEADERS += \
    src/appcontroller.h \
    src/appconfig.h \
    src/climawidget.h \
    src/logger.h \
    src/loginwindow.h \
    src/mainwindow.h \
    src/models.h \
    src/networkservice.h \
    src/pantalla.h

FORMS += \
    ui/login.ui

RESOURCES += \
    resources/resources.qrc
