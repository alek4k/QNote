#-------------------------------------------------
#
# Project created by QtCreator 2019-06-13T22:33:54
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = QNote
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
        imgnote.cpp \
        main.cpp \
        mainwindow.cpp \
        nota.cpp \
        note.cpp \
        notelistwidget.cpp \
        notewidget.cpp \
        simplenote.cpp \
        todoitem.cpp \
        todolistwidget.cpp \
        todonote.cpp

HEADERS += \
        container.h \
        imgnote.h \
        mainwindow.h \
        nota.h \
        note.h \
        notelistwidget.h \
        notewidget.h \
        simplenote.h \
        todoitem.h \
        todolistwidget.h \
        todonote.h

FORMS +=

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
