#-------------------------------------------------
#
# Project created by QtCreator 2019-12-29T12:48:11
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = OGLlesson8
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

CONFIG += c++11

SOURCES += \
        main.cpp \
    objectengine3d.cpp \
    skyboxnew.cpp \
        widget.cpp \
    simpleobject3d.cpp \
    group3d.cpp \
    camera3d.cpp \
    skybox.cpp \
    material.cpp \
    materiallibrary.cpp

HEADERS += \
    objectengine3d.h \
    skyboxnew.h \
        widget.h \
    simpleobject3d.h \
    transformational.h \
    group3d.h \
    camera3d.h \
    skybox.h \
    material.h \
    materiallibrary.h

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES +=

RESOURCES += \
    shaders.qrc \
    textures.qrc \
    models.qrc

win32-g* {
    LIBS += -lopengl32
}
win32-msvc* {
    LIBS += opengl32.lib
}
