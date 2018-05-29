#-------------------------------------------------
#
# Project created by QtCreator 2018-05-29T10:09:23
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = urhoqt1
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS
CONFIG += no_keywords console
# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0
URHO_HOME= D:/masteraplikasi/transferh11nov/urho3d/msvc
win32-msvc* {
    WINLIBS += kernel32.lib user32.lib gdi32.lib winspool.lib shell32.lib 
    WINLIBS += ole32.lib uuid.lib comdlg32.lib advapi32.lib dbghelp.lib imm32.lib ws2_32.lib 
     WINLIBS += oleaut32.lib version.lib Winmm.lib opengl32.lib



    CONFIG(debug,debug|release){
        INCLUDEPATH += $$URHO_HOME/debug/include $$URHO_HOME/debug/include/Urho3D \
        $$URHO_HOME/release/include/Urho3D/ThirdParty
        LIBS += $$URHO_HOME/debug/lib/Urho3D_d.lib $$WINLIBS
    }
    CONFIG( release,debug|release){
        INCLUDEPATH += $$URHO_HOME/release/include $$URHO_HOME/release/include/Urho3D \
        $$URHO_HOME/release/include/Urho3D/ThirdParty
        LIBS += $$URHO_HOME/release/lib/Urho3D.lib $$WINLIBS
    }
}

SOURCES += \
        main.cpp \
        mainwindow.cpp \
    urho3dwidget.cpp

HEADERS += \
        mainwindow.h \
    urho3dwidget.h

FORMS += \
        mainwindow.ui
