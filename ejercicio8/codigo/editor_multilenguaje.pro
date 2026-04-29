QT += widgets

CONFIG += c++17
TEMPLATE = app
TARGET = editor_multilenguaje

SOURCES += \
    src/main.cpp \
    src/appcontroller.cpp \
    src/appconfig.cpp \
    src/editorprincipal.cpp \
    src/loginscreen.cpp \
    src/logger.cpp \
    src/modobloqueado.cpp \
    src/pantalla.cpp \
    src/validadorsintaxis.cpp

HEADERS += \
    src/appcontroller.h \
    src/appconfig.h \
    src/editorprincipal.h \
    src/loginscreen.h \
    src/logger.h \
    src/models.h \
    src/modobloqueado.h \
    src/pantalla.h \
    src/validadorsintaxis.h

RESOURCES += \
    resources/resources.qrc
