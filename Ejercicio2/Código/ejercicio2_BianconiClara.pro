QT       += core gui network

# Esto permite que la interfaz se vea correctamente en versiones modernas de Qt
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# Listamos cada archivo UNA sola vez
SOURCES += \
    Monitorwidget.cpp \
    main.cpp \
    vpsmonitor.cpp

HEADERS += \
    Monitorwidget.h \
    vpsmonitor.h

FORMS += \
    Monitorwidget.ui

# Esta línea ayuda a que el ejecutable se guarde en una carpeta organizada
DESTDIR = bin