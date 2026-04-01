# Created by and for Qt Creator This file was created for editing the project sources only.
# You may attempt to use it for building too, by modifying this file here.

cmake_minimum_required(VERSION 3.5)
project(Ejercicio1)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
find_package(Qt5 COMPONENTS Core Gui Widgets REQUIRED)

include_directories(
    .
    build/Desktop_Qt_6_10_2_MinGW_64_bit-Debug/ejercicio1_autogen
    build/Desktop_Qt_6_10_2_MinGW_64_bit-Debug/ejercicio1_autogen/include
)

set (SRCS
    build/Desktop_Qt_6_10_2_MinGW_64_bit-Debug/CMakeFiles/3.30.5/CompilerIdCXX/CMakeCXXCompilerId.cpp
    build/Desktop_Qt_6_10_2_MinGW_64_bit-Debug/ejercicio1_autogen/EWIEGA46WW/moc_appcontroller.cpp
    build/Desktop_Qt_6_10_2_MinGW_64_bit-Debug/ejercicio1_autogen/EWIEGA46WW/moc_dashboardwidget.cpp
    build/Desktop_Qt_6_10_2_MinGW_64_bit-Debug/ejercicio1_autogen/EWIEGA46WW/moc_loginwidget.cpp
    build/Desktop_Qt_6_10_2_MinGW_64_bit-Debug/ejercicio1_autogen/EWIEGA46WW/moc_mainwindow.cpp
    build/Desktop_Qt_6_10_2_MinGW_64_bit-Debug/ejercicio1_autogen/include/ui_mainwindow.h
    build/Desktop_Qt_6_10_2_MinGW_64_bit-Debug/ejercicio1_autogen/moc_predefs.h
    build/Desktop_Qt_6_10_2_MinGW_64_bit-Debug/ejercicio1_autogen/mocs_compilation.cpp
    appcontroller.cpp
    appcontroller.h
    dashboardwidget.cpp
    dashboardwidget.h
    filemanager.cpp
    filemanager.h
    historial.txt
    loginwidget.cpp
    loginwidget.h
    main.cpp
    mainwindow.cpp
    mainwindow.h
    mainwindow.ui
    task.cpp
    task.h
    tasks.txt
    usuario.txt
    usuarios.txt
)


add_executable(${CMAKE_PROJECT_NAME} ${SRCS})

target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE Qt5::Core Qt5::Gui Qt5::Widgets)
